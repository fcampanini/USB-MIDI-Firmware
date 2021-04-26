/*
* main.c
*
* Created: 03/01/2021 00:48:14
*  Author: Filippo Campanini
*/

#include <atmel_start.h>
#include "MIDIdf.h"
#include "MIDIdf_desc.h"
#include "RingBuffer.h"


/*################################ PROTOTYPES AND VARIABLES##################################*/

volatile bool test;

/*------------------------------TIMER_SERVICE---------------------------------------------*/
//This TC (TC0) provide asynch counting for some service function such as heart beat LED
//and data in and out LED blinks
static struct timer_task TIMER_Service_task1, TIMER_Service_task2 ;
static void TIMER_Service_task1_cb(const struct timer_task *const timer_task);
static void TIMER_Service_task2_cb(const struct timer_task *const timer_task);
static void TIMER_Service_Load(void);
/*----------------------------------------------------------------------------------------*/

/*---------------------------------TIMER_USB_TX-------------------------------------------*/
// This TC (TC2) is providing asynch counting for TX USB MIDI transactions
static struct timer_task TIMER_USB_TX_task1;
static void TIMER_USB_TX_task1_cb(const struct timer_task *const timer_task);
static void TIMER_USB_TX_Load(void);
/*----------------------------------------------------------------------------------------*/

/*-----------------------------------USB-MIDI---------------------------------------------*/
volatile bool isUsbRxLED, isUsbTxLED;
uint8_t usbRxIndex, usbTxIndex;
volatile bool isBtnDown;
struct Buffer MIDI_ring_buffer_rx = {{}, 0, 0}; // Ring buffer for RX incoming data
struct Buffer MIDI_ring_buffer_tx = {{}, 0, 0}; // Ring buffer for TX outgoing data

/** USB Code Index Number. */
enum usbMIDI_CIN {
	USB_MIDI_TWO_BYTES_SYS_COM		=	2,
	USB_MIDI_THREE_BYTES_SYS_COM	=	3,
	USB_MIDI_SYSEX_ST_CONT			=	4,
	USB_MIDI_ONE_BYTE_SYSEX_END		=	5,
	USB_MIDI_TWO_BYTES_SYSEX_END	=	6,
	USB_MIDI_THREE_BYTES_SYSEX_END	=	7,
	USB_MIDI_NOTE_OFF				=	8,
	USB_MIDI_NOTE_ON				=	9,
	USB_MIDI_POLY_KEY_PRESS			=	10,
	USB_MIDI_CONTROL_CNG			=	11,
	USB_MIDI_PROGRAM_CNG			=	12,
	USB_MIDI_CH_PRESSURE			=	13,
	USB_MIDI_PITCH_BEND				=	14,
	USB_MIDI_ONE_BYTE				=	15
};
/** MIDI message types */
enum MIDI_message_type
{
	PITCHBEND		=	0,
	CONTROL_CHANGE	=	1,
	AFTERTOUCH		=	2,
	NOTEON			=	3,
	SYSEX			=	4
};

/**\Load USB Feature*/
void USBMIDI_Load(void);
/* Device descriptors and Configuration descriptors list. */
static uint8_t single_desc_bytes[] = {MIDI_DESCES_LS_FS};

//Set the address for the start and the end of the descriptor
static struct usbd_descriptors single_desc[] = {{single_desc_bytes, single_desc_bytes + sizeof(single_desc_bytes)}};
/** Ctrl endpoint buffer */
static uint8_t ctrl_buffer[64];

/** Loop back buffer for data validation */
COMPILER_ALIGNED(4)
static uint8_t main_data_buffer[1024];

//Structure containing the content of SYSEX message 
//already cleaned up from USB overhead bytes
struct sysex_message{
	uint8_t buf[64];
	uint8_t length;
};
struct sysex_message sysex;	
volatile uint32_t sysex_size;

//Structure containing NoteOn messages
//already cleaned up from USB overhead bytes
struct noteon_message
{
	uint8_t buf[3];
};
struct noteon_message noteon;

//Structure containing Control Change messages
//already cleaned up from USB overhead bytes
struct control_change_message
{
	uint8_t buf[3];
};
struct control_change_message control_change;

//Structure containing Aftertouch messages
//already cleaned up from USB overhead bytes
struct aftertouch_message
{
	uint8_t buf[2];
};
struct aftertouch_message aftertouch;

//Structure containing Pitchbend messages
//already cleaned up from USB overhead bytes
struct pitchbend_message
{
	uint8_t buf[3];
};
struct pitchbend_message pitchbend;
/*----------------------------------------------------------------------------------------*/

/*-----------------------------------DISPATCHING---------------------------------------------*/
/** receive a message and the message type and dispatch the MIDI message */
void dispatch_message(void* message, enum MIDI_message_type message_type);
/*----------------------------------------------------------------------------------------*/


/*####################################### MAIN #############################################*/
int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	TIMER_Service_Load();
	TIMER_USB_TX_Load();
	USBMIDI_Load();
	
	gpio_set_pin_level(LED_PWR, true);
	
	//Message writing to unblock the driver at the startup
	delay_ms(1);
	MIDIdf_write(USB_EP_TYPE_BULK, main_data_buffer, 64);
	
	//Reading to unblock the driver at the startup
	delay_ms(1);
	MIDIdf_read(USB_EP_TYPE_BULK, main_data_buffer, 64); 	

	/* Replace with your application code */
	while (1) 
	{
		if(gpio_get_pin_level(BTN_TEST) == false && isBtnDown == false)
		{
			delay_ms(1);
			isBtnDown = true;
			
			uint8_t testData[4] = {0x9, 0x90, 0x09, 0x7F};
			
			for(uint8_t i = 0; i < 4; i++)
			{
				bufferWrite(&MIDI_ring_buffer_tx, testData[i]);
			}
		}
		else if(gpio_get_pin_level(BTN_TEST) == true && isBtnDown == true)
		{
			delay_ms(1);
			isBtnDown = false;
		}
		
		if(!isBufferEmpty(&MIDI_ring_buffer_rx))
		{
			uint8_t usbPacket[4];
			for(uint8_t i = 0; i < 4; i ++)
			{
				bufferRead(&MIDI_ring_buffer_rx, &usbPacket[i]);
			}
			
			
			uint8_t start_sysex_byte;
			switch(usbPacket[0])
			{
				case USB_MIDI_NOTE_ON:
					//MIDIdf_write(USB_EP_TYPE_BULK, usbPacket, 4);
					
					noteon.buf[0] = usbPacket[1];
					noteon.buf[1] = usbPacket[2];
					noteon.buf[2] = usbPacket[3];
					
					dispatch_message(&noteon, NOTEON);
					
				break;
				
				case USB_MIDI_NOTE_OFF:
					//MIDIdf_write(USB_EP_TYPE_BULK, usbPacket, 4);
				break;

				case USB_MIDI_TWO_BYTES_SYS_COM:
					//MIDIdf_write(USB_EP_TYPE_BULK, usbPacket, 4);
				break;
				
				case USB_MIDI_THREE_BYTES_SYS_COM:
					//MIDIdf_write(USB_EP_TYPE_BULK, usbPacket, 4);
				break;
				
				case USB_MIDI_POLY_KEY_PRESS:
					//MIDIdf_write(USB_EP_TYPE_BULK, usbPacket, 4);
				break;
				
				case USB_MIDI_CONTROL_CNG:
					//MIDIdf_write(USB_EP_TYPE_BULK, usbPacket, 4);
					
					control_change.buf[0] = usbPacket[1];
					control_change.buf[1] = usbPacket[2];
					control_change.buf[2] = usbPacket[3];
					
					dispatch_message(&control_change, CONTROL_CHANGE);
				break;
				
				case USB_MIDI_PROGRAM_CNG:
					//MIDIdf_write(USB_EP_TYPE_BULK, usbPacket, 4);
				break;
				
				case USB_MIDI_CH_PRESSURE:
					//MIDIdf_write(USB_EP_TYPE_BULK, usbPacket, 4);
					
					aftertouch.buf[0] = usbPacket[1];
					aftertouch.buf[1] = usbPacket[2];
					
					dispatch_message(&aftertouch, AFTERTOUCH);
				break;
				
				case USB_MIDI_PITCH_BEND:
					//MIDIdf_write(USB_EP_TYPE_BULK, usbPacket, 4);
					
					pitchbend.buf[0] = usbPacket[1];
					pitchbend.buf[1] = usbPacket[2];
					pitchbend.buf[2] = usbPacket[3];
					
					dispatch_message(&pitchbend, PITCHBEND);

				break;
				
				case USB_MIDI_ONE_BYTE:
					//MIDIdf_write(USB_EP_TYPE_BULK, usbPacket, 4);
				break;
				
				case USB_MIDI_SYSEX_ST_CONT:
					//MIDIdf_write(USB_EP_TYPE_BULK, usbPacket, 4);
					
					start_sysex_byte = ((sysex_size / 4) * 3);
					
					for(uint8_t i = 0; i < 3; i ++)
					{
						sysex.buf[start_sysex_byte + i] = usbPacket[i + 1];
					}
					
					sysex_size += 4;
				break;
				
				case USB_MIDI_ONE_BYTE_SYSEX_END:
					
					start_sysex_byte = ((sysex_size / 4) * 3);
					
					for(uint8_t i = 0; i < 3; i ++)
					{
						sysex.buf[start_sysex_byte + i] = usbPacket[i + 1];
					}
					
					sysex_size += 4;
					sysex.length = (sysex_size - (sysex_size / 4) - 2);
					sysex_size = 0;
					
					dispatch_message(&sysex, SYSEX);
				break;
				
				case USB_MIDI_TWO_BYTES_SYSEX_END:
					
					start_sysex_byte = ((sysex_size / 4) * 3);
					
					for(uint8_t i = 0; i < 3; i ++)
					{
						sysex.buf[start_sysex_byte + i] = usbPacket[i + 1];
					}
					
					sysex_size += 4;
					sysex.length = (sysex_size - (sysex_size / 4) - 1);
					sysex_size = 0;
					
					dispatch_message(&sysex, SYSEX);
				break;
				
				case USB_MIDI_THREE_BYTES_SYSEX_END:
					
					start_sysex_byte = ((sysex_size / 4) * 3);
					
					for(uint8_t i = 0; i < 3; i ++)
					{
						sysex.buf[start_sysex_byte + i] = usbPacket[i + 1];
					}
					
					sysex_size += 4;
					sysex.length = sysex_size - (sysex_size / 4);
					sysex_size = 0;
					
					dispatch_message(&sysex, SYSEX);
				break;
			}
		}
	}
}


/*#################################### EVENT HANDLERS ######################################*/

/*------------------------------TIMER_SERVICE---------------------------------------------*/
static void TIMER_Service_task1_cb(const struct timer_task *const timer_task)
{
	gpio_toggle_pin_level(LED_PULSE);
}

static void TIMER_Service_task2_cb(const struct timer_task *const timer_task)
{
	if(isUsbRxLED == true && usbRxIndex < 3)
	{
		gpio_toggle_pin_level(LED_USB_RX);
		usbRxIndex++;
	}
	else
	{
		gpio_set_pin_level(LED_USB_RX, false);
		usbRxIndex = 0;
		isUsbRxLED = false;
	}
	
	if(isUsbTxLED == true && usbTxIndex < 3)
	{
		gpio_toggle_pin_level(LED_USB_TX);
		usbTxIndex++;
	}
	else
	{
		gpio_set_pin_level(LED_USB_TX, false);
		usbTxIndex = 0;
		isUsbTxLED = false;
	}
}
/*----------------------------------------------------------------------------------------*/

/*------------------------------TIMER_USB_TX---------------------------------------------*/
static void TIMER_USB_TX_task1_cb(const struct timer_task *const timer_task)
{
	if(!isBufferEmpty(&MIDI_ring_buffer_tx))
	{
		uint8_t buf[4];
		for(uint8_t i = 0; i < 4; i ++)
		{
			bufferRead(&MIDI_ring_buffer_tx, &buf[i]);
		}
		MIDIdf_write(USB_EP_TYPE_BULK, buf, 4);
	}
}
/*----------------------------------------------------------------------------------------*/

/*-----------------------------------USB-MIDI---------------------------------------------*/
/**
 * \brief Ctrl endpointer out transfer callback function
 * \param[in] count data amount to transfer
 */
static void usb_device_cb_ctrl_out(uint16_t count)
{
	uint16_t recv_cnt = 1024;

	if (recv_cnt > count) {
		recv_cnt = count;
	}
	MIDIdf_read(USB_EP_TYPE_CONTROL, main_data_buffer, recv_cnt);
	MIDIdf_read(USB_EP_TYPE_BULK, main_data_buffer, 1024);
}

/**
 * \brief Ctrl endpointer in transfer callback function
 * \param[in] count data amount to transfer
 */
static void usb_device_cb_ctrl_in(uint16_t count)
{
	uint16_t recv_cnt = 1024;

	if (recv_cnt > count) {
		recv_cnt = count;
	}
	MIDIdf_write(USB_EP_TYPE_CONTROL, main_data_buffer, recv_cnt);
}

/**
 * \brief Bulk endpointer out transfer callback function
 * \param[in] ep Endpointer number
 * \param[in] rc Transfer status codes
 * \param[in] count Data amount to transfer
 * \return Operation status.
 */
static bool usb_device_cb_bulk_out(const uint8_t ep, const enum usb_xfer_code rc, const uint32_t count)
{	
	for(int i = 0; i < count; i++)
	{
		bufferWrite(&MIDI_ring_buffer_rx, main_data_buffer[i]);
	}
	
	isUsbRxLED = true;
	MIDIdf_read(USB_EP_TYPE_BULK, main_data_buffer, 64);
	return false;
}

/**
 * \brief Bulk endpointer in transfer callback function
 * \param[in] ep Endpointer number
 * \param[in] rc Transfer status codes
 * \param[in] count Data amount to transfer
 * \return Operation status.
 */
static bool usb_device_cb_bulk_in(const uint8_t ep, const enum usb_xfer_code rc, const uint32_t count)
{	
	isUsbTxLED = true;
	return false;
}
/*----------------------------------------------------------------------------------------*/

/*#################################### PRIVATE FUNCTIONS ######################################*/

/*------------------------------TIMER_SERVICE---------------------------------------------*/
static void TIMER_Service_Load(void)
{
	TIMER_Service_task1.interval = 1000;
	TIMER_Service_task1.cb       = TIMER_Service_task1_cb;
	TIMER_Service_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_Service_task2.interval = 30;
	TIMER_Service_task2.cb       = TIMER_Service_task2_cb;
	TIMER_Service_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_Service, &TIMER_Service_task1);
	timer_add_task(&TIMER_Service, &TIMER_Service_task2);
	timer_start(&TIMER_Service);
}
/*----------------------------------------------------------------------------------------*/

/*------------------------------TIMER_USB_TX----------------------------------------------*/
static void TIMER_USB_TX_Load(void)
{
	TIMER_USB_TX_task1.interval = 3;
	TIMER_USB_TX_task1.cb       = TIMER_USB_TX_task1_cb;
	TIMER_USB_TX_task1.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_USB_TX, &TIMER_USB_TX_task1);
	timer_start(&TIMER_USB_TX);
}
/*----------------------------------------------------------------------------------------*/

/*-------------------------------DISPATCHER FOR MIDI MESSAGES-----------------------------*/
/**
* Dispatches all the MIDI messages already filtered out from USB protocol overhead data
* to the correct outbound activities
**/
void dispatch_message(void* message, enum MIDI_message_type message_type)
{
	switch(message_type)
	{
	case PITCHBEND: ;
		struct pitchbend_message* _pitchbend = (struct pitchbend_message*) message;
	break;
	
	case AFTERTOUCH: ;
		struct aftertouch_message* _aftertouch = (struct aftertouch_message*) message;
	break;
	
	case CONTROL_CHANGE: ;
		struct control_change_message* _control_change = (struct control_change_message*) message;
	break;
	
	case NOTEON: ;
		struct noteon_message* _noteon = (struct noteon_message*) message;
	break;
	
	case SYSEX: ;
		struct sysex_message* _sysex = (struct sysex_message*) message;
	break;
	}
}
/*----------------------------------------------------------------------------------------*/

/*-----------------------------------USB-MIDI---------------------------------------------*/
/**
 * Example of MIDI Function.
 * \note
 * In this example, we will use a PC as a USB host:
 * - Connect the DEBUG USB on XPLAINED board to PC for program download.
 * - Connect the TARGET USB on XPLAINED board to PC for running program.
 * The example uses a MIDI class which implements a loopback on the following
 * endpoints types: control and bulk.
 * The host application developed on libusb library is provided with application
 * note AVR4901. Except for control endpoint, other type endpoints are optional.
 * Application always run from control endpoint firstly.
 */
void USBMIDI_Load(void)
{
	/* usb stack init */
	usbdc_init(ctrl_buffer);

	/* usbdc_register_funcion inside */
	MIDIdf_init();
	MIDIdf_register_callback(MIDIDF_CB_CTRL_READ_REQ, (FUNC_PTR)usb_device_cb_ctrl_out);
	MIDIdf_register_callback(MIDIDF_CB_CTRL_WRITE_REQ, (FUNC_PTR)usb_device_cb_ctrl_in);
	MIDIdf_register_callback(MIDIDF_CB_BULK_READ, (FUNC_PTR)usb_device_cb_bulk_out);
	MIDIdf_register_callback(MIDIDF_CB_BULK_WRITE, (FUNC_PTR)usb_device_cb_bulk_in);

	usbdc_start(single_desc);
	usbdc_attach();

	while (!MIDIdf_is_enabled()) {
		// wait MIDI application to be installed
	};
	gpio_set_pin_level(LED_USB_EN, true);
}
/*----------------------------------------------------------------------------------------*/

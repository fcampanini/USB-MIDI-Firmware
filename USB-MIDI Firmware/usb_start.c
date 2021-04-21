/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file or main.c
 * to avoid loosing it when reconfiguring.
 */
#include "atmel_start.h"
#include "usb_start.h"
#include "MIDIdf.h"
#include "MIDIdf_desc.h"

static uint8_t single_desc_bytes[] = {
    /* Device descriptors and Configuration descriptors list. */
    MIDI_DESCES_LS_FS};

static struct usbd_descriptors single_desc[] = {{single_desc_bytes, single_desc_bytes + sizeof(single_desc_bytes)}
#if CONF_USBD_HS_SP
                                                ,
                                                {NULL, NULL}
#endif
};

/** Ctrl endpoint buffer */
static uint8_t ctrl_buffer[64];

/** Loop back buffer for data validation */
COMPILER_ALIGNED(4)
static uint8_t main_buf_loopback[1024];

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
	MIDIdf_read(USB_EP_TYPE_CONTROL, main_buf_loopback, recv_cnt);
	MIDIdf_read(USB_EP_TYPE_BULK, main_buf_loopback, 1024);
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
	MIDIdf_write(USB_EP_TYPE_CONTROL, main_buf_loopback, recv_cnt);
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
	MIDIdf_write(USB_EP_TYPE_BULK, main_buf_loopback, count);

	isUsbRxLED = true;

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
	MIDIdf_read(USB_EP_TYPE_BULK, main_buf_loopback, count);
	
	isUsbTxLED = true;

	return false;
}

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
	gpio_set_pin_level(LED_ORG, true);
}

void usb_init(void)
{
	USBMIDI_Load();	
}


/*
	RX_payload_count = count;
	
	MIDIdf_read(USB_EP_TYPE_BULK, main_buf_loopback, count);
	
	if(main_buf_loopback[2] == 0x06)
	{
		gpio_set_pin_level(LED_DB_1, true);
	}
	else
	{
		gpio_set_pin_level(LED_DB_1, false);
	}
	
	if(main_buf_loopback[2] == 0x07)
	{
		gpio_set_pin_level(LED_DB_2, true);
	}
	else
	{
		gpio_set_pin_level(LED_DB_2, false);
	}
	
	isUsbRx = true;
*/
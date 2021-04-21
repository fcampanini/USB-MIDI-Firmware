/*
 * usb_protocol_MIDI.h
 *
 * Created: 01/01/2021 23:07:56
 *  Author: Filippo Campanini
 */ 


#ifndef USB_PROTOCOL_MIDI_H_
#define USB_PROTOCOL_MIDI_H_


/**
 * usb_MIDI_protocol USB MIDI Class definitions
 *
 */

/**
 * \name MIDI class values
 */
#define MIDI_CLASS 0x01
#define MIDI_SUBCLASS 0x01
#define MIDI_PROTOCOL 0x00

/**
 * \brief Class Specific Audio Device Class: MIDIStreaming USB descriptor types
 */

enum usb_cs_MS_interface_subtypes
{
	MS_DESCRIPTOR_UNDEFINED					= 0x00,
	MS_HEADER								= 0x01,
	MIDI_IN_JACK							= 0x02,
	MIDI_OUT_JACK							= 0x03,
	ELEMENT									= 0x04	
};

enum usb_cs_MS_endpoint_subtypes
{
	DESCRIPTOR_UNIDENTIFIED					= 0x00,
	MS_GENERAL								= 0x01	
};

enum usb_cs_MIDI_jack_types
{
	JACK_TYPE_UNIDENTIFIED					= 0x00,
	EMBEDDED								= 0x01,
	EXTERNAL								= 0x02
};

enum usb_cs_MIDI_endpoint_ctrl_selector
{
	EP_CONTROL_UNDEFINED					= 0x00,
	ASSOCIATION_CONTROL						= 0x01	
};

enum usb_cs_MIDI_descriptor_subclass
{
	USB_CS_DT_SUBC_AUDIO					= 0x01,
	USB_CS_DT_SUBC_MIDISTREAMING			= 0x03
};

enum usb_cs_MIDI_descriptor_type
{
	USB_CS_DT_INTERFACE						= 0x24,
	USB_CS_DT_ENDPOINT						= 0x25
};

enum usb_cs_MIDI_descriptor_subtype
{
	USB_CS_DT_SUBT_HEADER					= 0x01,
	USB_CS_DT_SUBT_JACKIN					= 0x02,
	USB_CS_DT_SUBT_JACKOUT					= 0x03
	
};

enum usb_cs_MIDI_jack_descriptor_type
{
	USB_CS_JACK_EMBEDDED					= 0x01,
	USB_CS_JACK_EXTERNAL					= 0x02
};

enum usb_cs_MIDI_ep_descriptor_subtype
{
	USB_CS_EP_GENERAL						= 0x01
};


/**
 * \brief Class Specific Audio Control USB interface descriptor structure
 */
typedef struct usb_cs_AC_iface_desc {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint16_t bcdADC;
	uint16_t wTotalLength;
	uint8_t bInCollection;
	uint8_t baInterfaceNr;
} usb_cs_AC_iface_desc;

/**
 * \brief Class Specific MIDIStreaming Header USB interface descriptor structure
 */
typedef struct usb_cs_MS_Header_iface_desc {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint16_t bcdMSC;
	uint16_t wTotalLength;
} usb_cs_MS_Header_iface_desc ;

/**
 * \brief Class Specific MIDIStreaming MIDI IN Jack USB descriptor structure
 */
typedef struct usb_cs_MS_MIDIin_Jack_iface_desc {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bJackType;
	uint8_t bJackID;
	uint8_t iJack;
} usb_cs_MS_MIDIin_Jack_iface_desc;

/**
 * \brief Class Specific MIDIStreaming MIDI OUT Jack USB descriptor structure
 */
typedef struct usb_cs_MS_MIDIout_Jack_iface_desc {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bJackType;
	uint8_t bJackID;
	uint8_t bNrInputPins;
	uint8_t baSourceID; // should be flexible (FAM)
	uint8_t BASourcePin; // should be flexible (FAM)
} usb_cs_MS_MIDIout_Jack_iface_desc;


/**
 * \brief Class Specific MIDIStreaming BULK endpoint USB descriptor structure
 */
typedef struct usb_cs_MS_BULK_ep_desc {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bNumbEmbMIDIJack;
	uint8_t baAssocJackID; // should be flexible (FAM)
} usb_cs_MS_BULK_ep_desc;


/** \name Macros to build USB class specific descriptors */
/*@{*/

/** Build bytes for USB CS AC interface descriptor. */
#define USB_CS_AC_IFACE_DESC_BYTES(bLength,                                                                 \
							 bDescriptorType,                                                               \
							 bDescriptorSubtype,                                                            \
							 bcdADC,                                                                        \
							 wTotalLength,                                                                  \
							 bInCollection,                                                                 \
							 baInterfaceNr)                                                                 \
	bLength, bDescriptorType, bDescriptorSubtype, LE_BYTE0(bcdADC),											\
	LE_BYTE1(bcdADC), LE_BYTE0(wTotalLength), LE_BYTE1(wTotalLength),										\
	bInCollection, baInterfaceNr
	
#define USB_CS_AC_IFACE_DESC_LEN 9
	
/** Build bytes for USB CS MS Header interface descriptor. */
#define USB_CS_MS_IFACE_DESC_BYTES(bLength,                                                                 \
								   bDescriptorType,                                                         \
								   bDescriptorSubtype,                                                      \
								   bcdMSC,                                                                  \
								   wTotalLength)															\
	bLength, bDescriptorType, bDescriptorSubtype, LE_BYTE0(bcdMSC),											\
	LE_BYTE1(bcdMSC), LE_BYTE0(wTotalLength), LE_BYTE1(wTotalLength)
		
#define USB_CS_MS_IFACE_DESC_LEN 7

/** Build bytes for USB CS MS MIDI IN descriptor. */
#define USB_CS_JACKIN_IFACE_DESC_BYTES(bLength,                                                             \
									   bDescriptorType,                                                     \
									   bDescriptorSubtype,                                                  \
									   bJackType,                                                           \
									   bJackID,																\
									   iJack)																\
	bLength, bDescriptorType, bDescriptorSubtype, bJackType,												\
	bJackID, iJack
	
#define USB_CS_JACKIN_IFACE_DESC_LEN 6
	
/** Build bytes for USB CS MS MIDI OUT descriptor. */
#define USB_CS_JACKOUT_IFACE_DESC_BYTES(bLength,                                                            \
									   bDescriptorType,                                                     \
									   bDescriptorSubtype,                                                  \
									   bJackType,															\
									   bJackID,																\
									   bNrInputPins,														\
									   baSourceID,															\
									   BASourcePin,															\
									   iJack)																\
		bLength, bDescriptorType, bDescriptorSubtype, bJackType,											\
		bJackID, bNrInputPins, baSourceID, BASourcePin, iJack

#define USB_CS_JACKOUT_IFACE_DESC_LEN 9


/** Build bytes for USB CS MS BULK endpoint descriptor. */
#define USB_CS_MS_BULK_ENDP_DESC_BYTES(bLength,																\
									   bDescriptorType,                                                     \
									   bDescriptorSubtype,                                                  \
									   bNumbEmbMIDIJack,													\
									   baAssocJackID)														\
		bLength, bDescriptorType, bDescriptorSubtype, bNumbEmbMIDIJack,										\
		baAssocJackID
		
#define USB_CS_MS_BULK_ENDP_DESC_LEN 5


#endif /* USB_PROTOCOL_MIDI_H_ */

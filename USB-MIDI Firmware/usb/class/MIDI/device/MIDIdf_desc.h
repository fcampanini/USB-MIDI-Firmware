/*
 * MIDIf_desc.h
 *
 * Created: 02/01/2021 15:07:33
 *  Author: Filippo Campanini
 */ 


#ifndef MIDIF_DESC_H_
#define MIDIF_DESC_H_


#include "usb_protocol.h"
#include "usb_protocol_MIDI.h"
#include "usbd_config.h"

#define MIDI_DEV_DESC																															\
		USB_DEV_DESC_BYTES(CONF_USB_MIDI_BCDUSB,																								\
							0x00,																												\
							0x00,																												\
							0x00,																												\
							CONF_USB_MIDI_BMAXPKSZ0,																							\
							CONF_USB_MIDI_IDVENDER,																								\
							CONF_USB_MIDI_IDPRODUCT,																							\
							CONF_USB_MIDI_BCDDEVICE,																							\
							CONF_USB_MIDI_IMANUFACT,																							\
							CONF_USB_MIDI_IPRODUCT,																								\
							CONF_USB_MIDI_ISERIALNUM,																							\
							0x01)
							
#define MIDI_CFG_DESC																															\
		USB_CONFIG_DESC_BYTES(USB_CONFIG_DESC_LEN + USB_IFACE_DESC_LEN + USB_CS_AC_IFACE_DESC_LEN +												\
							  USB_IFACE_DESC_LEN + USB_CS_MS_IFACE_DESC_LEN +																	\
							  USB_CS_JACKIN_IFACE_DESC_LEN + USB_CS_JACKOUT_IFACE_DESC_LEN +													\
							  USB_CS_JACKIN_IFACE_DESC_LEN + USB_CS_JACKOUT_IFACE_DESC_LEN +													\
							  USB_ENDP_1_DESC_LEN + USB_CS_MS_BULK_ENDP_DESC_LEN + USB_ENDP_1_DESC_LEN +											\
							  USB_CS_MS_BULK_ENDP_DESC_LEN + CONF_USB_MIDI_IMANUFACT_STR_LEN +													\
							  CONF_USB_MIDI_IPRODUCT_STR_LEN + CONF_USB_VENDOR_LANGID_DESC_LEN,													\
							  2, 0x01, 0x00, CONF_USB_MIDI_BMATTRI, CONF_USB_MIDI_BMAXPOWER)

#define MIDI_IFC_DESC																															\
		USB_IFACE_DESC_BYTES(CONF_USB_MIDI_BIFCNUM, 0x00, 0x00, MIDI_CLASS, USB_CS_DT_SUBC_AUDIO, MIDI_PROTOCOL, CONF_USB_MIDI_IIFC),			\
		USB_CS_AC_IFACE_DESC_BYTES(USB_CS_AC_IFACE_DESC_LEN, USB_CS_DT_INTERFACE, USB_CS_DT_SUBT_HEADER,										\
								   0x100, USB_CS_AC_IFACE_DESC_LEN, 0x01, 0x01),																\
		USB_IFACE_DESC_BYTES(0x01, 0x00, 0x02, MIDI_CLASS, USB_CS_DT_SUBC_MIDISTREAMING, MIDI_PROTOCOL, CONF_USB_MIDI_IIFC),					\
		USB_CS_MS_IFACE_DESC_BYTES(USB_CS_MS_IFACE_DESC_LEN, USB_CS_DT_INTERFACE, USB_CS_DT_SUBT_HEADER,										\
								   0x100, USB_CS_MS_IFACE_DESC_LEN + USB_CS_JACKIN_IFACE_DESC_LEN +												\
								   USB_CS_JACKOUT_IFACE_DESC_LEN + USB_CS_JACKIN_IFACE_DESC_LEN +												\
								   USB_CS_JACKOUT_IFACE_DESC_LEN + USB_ENDP_1_DESC_LEN + USB_CS_MS_BULK_ENDP_DESC_LEN +							\
								   USB_ENDP_1_DESC_LEN + USB_CS_MS_BULK_ENDP_DESC_LEN),															\
		USB_CS_JACKIN_IFACE_DESC_BYTES(USB_CS_JACKIN_IFACE_DESC_LEN, USB_CS_DT_INTERFACE, USB_CS_DT_SUBT_JACKIN,								\
									   USB_CS_JACK_EMBEDDED, 0x01, 0x00),																		\
		USB_CS_JACKIN_IFACE_DESC_BYTES(USB_CS_JACKIN_IFACE_DESC_LEN, USB_CS_DT_INTERFACE, USB_CS_DT_SUBT_JACKIN,								\
									   USB_CS_JACK_EXTERNAL, 0x02, 0x00),																		\
		USB_CS_JACKOUT_IFACE_DESC_BYTES(USB_CS_JACKOUT_IFACE_DESC_LEN, USB_CS_DT_INTERFACE, USB_CS_DT_SUBT_JACKOUT,								\
										USB_CS_JACK_EMBEDDED, 0x03, 0x01, 0x02, 0x01, 0x00),													\
		USB_CS_JACKOUT_IFACE_DESC_BYTES(USB_CS_JACKOUT_IFACE_DESC_LEN, USB_CS_DT_INTERFACE, USB_CS_DT_SUBT_JACKOUT,								\
										USB_CS_JACK_EXTERNAL, 0x04, 0x01, 0x01, 0x01, 0x00),													\
		USB_ENDP_1_DESC_BYTES(CONF_USB_MIDI_BULKOUT_EPADDR, 0x02, 0x0040, 0x00),																	\
		USB_CS_MS_BULK_ENDP_DESC_BYTES(USB_CS_MS_BULK_ENDP_DESC_LEN, USB_CS_DT_ENDPOINT, USB_CS_EP_GENERAL, 0x01, 0x01),						\
		USB_ENDP_1_DESC_BYTES(CONF_USB_MIDI_BULKIN_EPADDR,0x02, 0x0040, 0x00),																	\
		USB_CS_MS_BULK_ENDP_DESC_BYTES(USB_CS_MS_BULK_ENDP_DESC_LEN, USB_CS_DT_ENDPOINT, USB_CS_EP_GENERAL, 0x01, 0x03)
		

#define VENDOR_STR_DESCES																														\
		CONF_USB_VENDOR_LANGID_DESC																												\
		CONF_USB_MIDI_IMANUFACT_STR_DESC																										\
		CONF_USB_MIDI_IPRODUCT_STR_DESC																											\
		CONF_USB_MIDI_ISERIALNUM_STR_DESC																										\
		CONF_USB_MIDI_ICONFIG_STR_DESC

/** USB Device descriptors and configuration descriptors */
#define MIDI_DESCES_LS_FS MIDI_DEV_DESC, MIDI_CFG_DESC, MIDI_IFC_DESC, VENDOR_STR_DESCES


#endif /* MIDIF_DESC_H_ */ 
/*
 * usbd_config.h
 *
 * Created: 01/01/2021 16:53:25
 *  Author: Filippo Campanini
 */ 


#ifndef USBD_CONFIG_H_
#define USBD_CONFIG_H_

// <q> High Speed Support
// <i> Enable high speed specific descriptors support, e.g., DeviceQualifierDescriptor and OtherSpeedConfiguration Descriptor.
// <i> High speed support require descriptors description array on start, for LS/FS and HS support in first and second place.
// <id> usbd_hs_sp
#ifndef CONF_USBD_HS_SP
#define CONF_USBD_HS_SP 0
#endif

// ---- USB Device Stack Vendor Options ----

// <e> Enable String Descriptors
// <id> usb_midi_str_en
#ifndef CONF_USB_VENDOR_STR_EN
#define CONF_USB_VENDOR_STR_EN 1
#endif
// <s> Language IDs
// <i> Language IDs in c format, split by comma (E.g., 0x0409 ...)
// <id> usb_midi_langid
#ifndef CONF_USB_VENDOR_LANGID
#define CONF_USB_VENDOR_LANGID "0x0409"
#endif

#ifndef CONF_USB_VENDOR_LANGID_DESC_LEN
#define CONF_USB_VENDOR_LANGID_DESC_LEN 4
#endif

#ifndef CONF_USB_VENDOR_LANGID_DESC
#define CONF_USB_VENDOR_LANGID_DESC CONF_USB_VENDOR_LANGID_DESC_LEN, 0x03, 0x09, 0x04,
#endif
// </e>

// <h> MIDI Class Device Descriptor

// <o> bcdUSB
// <0x0200=> USB 2.0 version
// <0x0210=> USB 2.1 version
// <id> usb_midi_bcdusb
#ifndef CONF_USB_MIDI_BCDUSB
#define CONF_USB_MIDI_BCDUSB 0x110
#endif

// <o> bMaxPackeSize0
// <0x0008=> 8 bytes
// <0x0010=> 16 bytes
// <0x0020=> 32 bytes
// <0x0040=> 64 bytes
// <id> usb_midi_bmaxpksz0
#ifndef CONF_USB_MIDI_BMAXPKSZ0
#define CONF_USB_MIDI_BMAXPKSZ0 0x0040
#endif

// <o> idVender <0x0000-0xFFFF>
// <id> usb_midi_idvender
#ifndef CONF_USB_MIDI_IDVENDER
#define CONF_USB_MIDI_IDVENDER 0x3eb
#endif

// <o> idProduct <0x0000-0xFFFF>
// <id> usb_midi_idproduct
#ifndef CONF_USB_MIDI_IDPRODUCT
#define CONF_USB_MIDI_IDPRODUCT 0x2433
#endif

// <o> bcdDevice <0x0000-0xFFFF>
// <id> usb_midi_bcddevice
#ifndef CONF_USB_MIDI_BCDDEVICE
#define CONF_USB_MIDI_BCDDEVICE 0x100
#endif

// <e> Enable string descriptor of iManufact
// <id> usb_midi_imanufact_en
#ifndef CONF_USB_MIDI_IMANUFACT_EN
#define CONF_USB_MIDI_IMANUFACT_EN 1
#endif

#ifndef CONF_USB_MIDI_IMANUFACT
#define CONF_USB_MIDI_IMANUFACT (CONF_USB_MIDI_IMANUFACT_EN * (CONF_USB_MIDI_IMANUFACT_EN))
#endif

// <s> Unicode string of iManufact
// <id> usb_midi_imanufact_str
#ifndef CONF_USB_MIDI_IMANUFACT_STR
#define CONF_USB_MIDI_IMANUFACT_STR "FCampanini"
#endif

#ifndef CONF_USB_MIDI_IMANUFACT_STR_LEN
#define CONF_USB_MIDI_IMANUFACT_STR_LEN 22
#endif

#ifndef CONF_USB_MIDI_IMANUFACT_STR_DESC
#define CONF_USB_MIDI_IMANUFACT_STR_DESC CONF_USB_MIDI_IMANUFACT_STR_LEN, 0x03, 'F', 0x00, 'C', 0x00, 'a', 0x00, 'm', 0x00, 'p', 0x00,			\
'a', 0x00, 'n', 0x00, 'i', 0x00, 'n', 0x00, 'i', 0x00,
#endif



// </e>

// <e> Enable string descriptor of iProduct
// <id> usb_midi_iproduct_en
#ifndef CONF_USB_MIDI_IPRODUCT_EN
#define CONF_USB_MIDI_IPRODUCT_EN 1
#endif

#ifndef CONF_USB_MIDI_IPRODUCT
#define CONF_USB_MIDI_IPRODUCT																													\
(CONF_USB_MIDI_IPRODUCT_EN * (CONF_USB_MIDI_IMANUFACT_EN + CONF_USB_MIDI_IPRODUCT_EN))
#endif

// <s> Unicode string of iProduct
// <id> usb_midi_iproduct_str
#ifndef CONF_USB_MIDI_IPRODUCT_STR
#define CONF_USB_MIDI_IPRODUCT_STR "MIDI PORT FCAMPANINI V0.5"
#endif

#ifndef CONF_USB_MIDI_IPRODUCT_STR_LEN
#define CONF_USB_MIDI_IPRODUCT_STR_LEN 52
#endif

#ifndef CONF_USB_MIDI_IPRODUCT_STR_DESC
#define CONF_USB_MIDI_IPRODUCT_STR_DESC																											\
CONF_USB_MIDI_IPRODUCT_STR_LEN, 0x03, 'M', 0x00, 'I', 0x00, 'D', 0x00, 'I', 0x00, ' ', 0x00, 'P', 0x00, 'O', 0x00, 'R', 0x00, 'T', 0x00, ' ',	\
0x00, 'F', 0x00, 'C', 0x00, 'a', 0x00, 'm', 0x00, 'p', 0x00, 'a', 0x00, 'n', 0x00, 'i', 0x00, 'n', 0x00, 'i', 0x00, ' ', 0x00, 'V', 0x00,		\
'0', 0x00, '.', 0x00, '5', 0x00,
#endif

// </e>

// <e> Enable string descriptor of iSerialNum
// <id> usb_midi_iserialnum_en
#ifndef CONF_USB_MIDI_ISERIALNUM_EN
#define CONF_USB_MIDI_ISERIALNUM_EN 0
#endif

#ifndef CONF_USB_MIDI_ISERIALNUM
#define CONF_USB_MIDI_ISERIALNUM																												\
(CONF_USB_MIDI_ISERIALNUM_EN																													\
* (CONF_USB_MIDI_IMANUFACT_EN + CONF_USB_MIDI_IPRODUCT_EN + CONF_USB_MIDI_ISERIALNUM_EN))
#endif

// <s> Unicode string of iSerialNum
// <id> usb_midi_iserialnum_str
#ifndef CONF_USB_MIDI_ISERIALNUM_STR
#define CONF_USB_MIDI_ISERIALNUM_STR "000000000000001"
#endif

#ifndef CONF_USB_MIDI_ISERIALNUM_STR_LEN
#define CONF_USB_MIDI_ISERIALNUM_STR_LEN 32
#endif

#ifndef CONF_USB_MIDI_ISERIALNUM_STR_DESC
#define CONF_USB_MIDI_ISERIALNUM_STR_DESC																										\
CONF_USB_MIDI_ISERIALNUM_STR_LEN, 0x03, '0', 0x00, '0', 0x00, '0', 0x00, '0', 0x00, '0', 0x00, '0', 0x00, '0', 0x00, '0', 0x00, '0', 0x00, '0', \
0x00, '0', 0x00, '0', 0x00, '0', 0x00, '0', 0x00, '1', 0x00,
#endif

// </e>

// <h> MIDI Class Configuration Descriptor

// <e> Enable string descriptor of iConfig
// <id> usb_midi_iconfig_en
#ifndef CONF_USB_MIDI_ICONFIG_EN
#define CONF_USB_MIDI_ICONFIG_EN 1
#endif

#ifndef CONF_USB_MIDI_ICONFIG
#define CONF_USB_MIDI_ICONFIG																													\
(CONF_USB_MIDI_ICONFIG_EN																														\
* (CONF_USB_MIDI_IMANUFACT_EN + CONF_USB_MIDI_IPRODUCT_EN + CONF_USB_MIDI_ISERIALNUM_EN															\
+ CONF_USB_MIDI_ICONFIG_EN))
#endif

// <s> Unicode string of iConfig
// <id> usb_midi_iconfig_str
#ifndef CONF_USB_MIDI_ICONFIG_STR
#define CONF_USB_MIDI_ICONFIG_STR ""
#endif

#ifndef CONF_USB_MIDI_ICONFIG_STR_DESC
#define CONF_USB_MIDI_ICONFIG_STR_DESC
#endif

// </e>

// <o> bmAttributes
// <0x80=> Bus power supply, not support for remote wakeup
// <0xA0=> Bus power supply, support for remote wakeup
// <0xC0=> Self powered, not support for remote wakeup
// <0xE0=> Self powered, support for remote wakeup
// <id> usb_midi_bmattri
#ifndef CONF_USB_MIDI_BMATTRI
#define CONF_USB_MIDI_BMATTRI 0x80
#endif

// <o> bMaxPower <0x00-0xFF>
// <id> usb_midi_bmaxpower
#ifndef CONF_USB_MIDI_BMAXPOWER
#define CONF_USB_MIDI_BMAXPOWER 0x32
#endif
// </h>

// <h> MIDI Interface Descriptor

// <o> bInterfaceNumber <0x00-0xFF>
// <id> usb_midi_bifcnum
#ifndef CONF_USB_MIDI_BIFCNUM
#define CONF_USB_MIDI_BIFCNUM 0x0
#endif

// <o> iInterface <0x00-0xFF>
// <id> usb_midi_iifc
#ifndef CONF_USB_MIDI_IIFC
#define CONF_USB_MIDI_IIFC 0x0
#endif

// <o> BULK IN Endpoint Address
// <0x81=> EndpointAddress = 0x81
// <0x82=> EndpointAddress = 0x82
// <0x83=> EndpointAddress = 0x83
// <0x84=> EndpointAddress = 0x84
// <0x85=> EndpointAddress = 0x85
// <0x86=> EndpointAddress = 0x86
// <0x87=> EndpointAddress = 0x87
// <id> usb_midi_bulkin_epaddr
// <i> Please make sure that the setting here is coincide with the endpoint setting in USB device driver.
#ifndef CONF_USB_MIDI_BULKIN_EPADDR
#define CONF_USB_MIDI_BULKIN_EPADDR 0x81
#endif

// <o> BULK IN Endpoint wMaxPacketSize
// <0x0008=> 8 bytes
// <0x0010=> 16 bytes
// <0x0020=> 32 bytes
// <0x0040=> 64 bytes
// <id> usb_midi_bulkin_maxpksz
// <i> Please make sure that the setting here is coincide with the endpoint setting in USB device driver.
#ifndef CONF_USB_MIDI_BULKIN_MAXPKSZ
#define CONF_USB_MIDI_BULKIN_MAXPKSZ 0x40
#endif

// <o> BULK OUT Endpoint Address
// <0x01=> EndpointAddress = 0x01
// <0x02=> EndpointAddress = 0x02
// <0x03=> EndpointAddress = 0x03
// <0x04=> EndpointAddress = 0x04
// <0x05=> EndpointAddress = 0x05
// <0x06=> EndpointAddress = 0x06
// <0x07=> EndpointAddress = 0x07
// <id> usb_midi_bulkout_epaddr
// <i> Please make sure that the setting here is coincide with the endpoint setting in USB device driver.
#ifndef CONF_USB_MIDI_BULKOUT_EPADDR
#define CONF_USB_MIDI_BULKOUT_EPADDR 0x01
#endif

// <o> BULK OUT Endpoint wMaxPacketSize
// <0x0008=> 8 bytes
// <0x0010=> 16 bytes
// <0x0020=> 32 bytes
// <0x0040=> 64 bytes
// <id> usb_midi_bulkout_maxpksz
// <i> Please make sure that the setting here is coincide with the endpoint setting in USB device driver.
#ifndef CONF_USB_MIDI_BULKOUT_MAXPKSZ
#define CONF_USB_MIDI_BULKOUT_MAXPKSZ 0x40
#endif

// </h>

// <<< end of configuration section >>>

#endif /* USBD_CONFIG_H_ */
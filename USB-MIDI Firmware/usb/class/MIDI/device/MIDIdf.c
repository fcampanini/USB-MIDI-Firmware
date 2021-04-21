/*
* MIDIdf.c
*
* Created: 03/01/2021 00:48:14
*  Author: Filippo Campanini
*/

#include "MIDIdf.h"

#define MIDI_VERSION 0x00000001u

#define MIDI_TRANS_OUT 0
#define MIDI_TRANS_IN 1

uint8_t bmRequestType, bRequest, test, endp;
uint16_t wIndex, vValue;

/** USB Device MIDI Function Specific Data */
struct MIDIdf_func_data {
	/** MIDI Interface information */
	uint8_t func_iface;
	/** MIDI Bulk Endpoint */
	uint8_t func_bulk_ep[2];
	/** MIDI Alternate setting */
	uint8_t func_alt_set;
	/** MIDI Enable Flag */
	bool enabled;
};

/* USB Device MIDI Function Instance */
static struct usbdf_driver _MIDIdf;

/* USB Device MIDI Function Data Instance */
static struct MIDIdf_func_data _MIDIdf_funcd;

/* USB Device MIDI Callback Function Pointer */
static MIDI_ctrl_cb_t	MIDI_ctrl_rd = NULL;
static MIDI_ctrl_cb_t	MIDI_ctrl_wr = NULL;
static FUNC_PTR         MIDI_bulk_rd = NULL;
static FUNC_PTR         MIDI_bulk_wr = NULL;

/**
* \brief Enable MIDI Function
* \param[in] drv	-	Pointer to USB device function driver
* \param[in] desc	-	Pointer to USB interface descriptor
* \return Operation status.
*/
static int32_t MIDIdf_enable(struct usbdf_driver *drv, struct usbd_descriptors *desc)
{
	uint8_t *        ifc, *ep, i;
	usb_iface_desc_t ifc_desc;
	usb_ep_desc_t    ep_desc;
	enum usb_ep_type ep_type;

	struct MIDIdf_func_data *func_data = (struct MIDIdf_func_data *)(drv->func_data);

	// STARTS DESCRIPTOR READING
	ifc = desc->sod;
	if (NULL == ifc)
	{
		return ERR_NOT_FOUND;
	}

	ifc_desc.bInterfaceNumber  = ifc[2];
	ifc_desc.bAlternateSetting = ifc[3];
	ifc_desc.bInterfaceClass   = ifc[5];

	if (MIDI_CLASS != ifc_desc.bInterfaceClass)
	{
		return ERR_NOT_FOUND;
	}

	if (func_data->func_iface != 0xFF && func_data->func_iface != ifc_desc.bInterfaceNumber)
	{
		return ERR_ALREADY_INITIALIZED;
	}
	else
	{
		if (func_data->func_iface == 0xFF || func_data->func_alt_set != ifc_desc.bAlternateSetting)
		{
			/* Install endpoints */
			for (i = 0; i < 2; i++)
			{
				ep = usb_find_ep_desc(usb_desc_next(desc->sod), desc->eod);
				if (NULL != ep)
				{
					desc->sod                = ep;
					ep_desc.bEndpointAddress = ep[2];
					ep_desc.bmAttributes     = ep[3];
					ep_desc.wMaxPacketSize   = usb_get_u16(ep + 4);
					ep_type                  = (enum usb_ep_type)(ep_desc.bmAttributes & USB_EP_TYPE_MASK);

					if (usb_d_ep_init(ep_desc.bEndpointAddress, ep_desc.bmAttributes, ep_desc.wMaxPacketSize)) // usb_d_ep_init returns 0 if success
					{
						return ERR_NOT_INITIALIZED;
					}

					switch (ep_type)
					{
						case USB_EP_TYPE_ISOCHRONOUS:
						break;
						case USB_EP_TYPE_BULK:
						if (ep_desc.bEndpointAddress & USB_EP_DIR_IN)
						{
							func_data->func_bulk_ep[MIDI_TRANS_IN] = ep_desc.bEndpointAddress;
							if (NULL != MIDI_bulk_wr)
							{
								usb_d_ep_register_callback(
								func_data->func_bulk_ep[MIDI_TRANS_IN], USB_D_EP_CB_XFER, MIDI_bulk_wr);
							}
						}
						else
						{
							func_data->func_bulk_ep[MIDI_TRANS_OUT] = ep_desc.bEndpointAddress;
							if (NULL != MIDI_bulk_rd)
							{
								usb_d_ep_register_callback(
								func_data->func_bulk_ep[MIDI_TRANS_OUT], USB_D_EP_CB_XFER, MIDI_bulk_rd);
							}
						}
						usb_d_ep_enable(ep_desc.bEndpointAddress);
						break;
						case USB_EP_TYPE_INTERRUPT:
						break;
						default:
						return ERR_INVALID_ARG;
					}
				}
				else
				{
					return ERR_INVALID_DATA;
				}
			}
			
			func_data->func_iface   = ifc_desc.bInterfaceNumber;
			func_data->func_alt_set = ifc_desc.bAlternateSetting;
			func_data->enabled      = (bool)func_data->func_alt_set;
		}
		
		_MIDIdf_funcd.enabled = true;
		return ERR_NONE;
	}
}

/**
* \brief Disable MIDI Function
* \param[in] drv	-	Pointer to USB device function driver
* \param[in] desc	-	Pointer to USB device descriptor
* \return Operation status.
*/
static int32_t MIDIdf_disable(struct usbdf_driver *drv, struct usbd_descriptors *desc)
{
	struct MIDIdf_func_data *func_data = (struct MIDIdf_func_data *)(drv->func_data);

	usb_iface_desc_t ifc_desc;

	if (desc)
	{
		ifc_desc.bInterfaceClass  = desc->sod[5];
		ifc_desc.bInterfaceNumber = desc->sod[2];
		if (ifc_desc.bInterfaceClass != MIDI_CLASS) {
			return ERR_NOT_FOUND;
		}
		if (func_data->enabled == true && func_data->func_iface != ifc_desc.bInterfaceNumber) {
			return ERR_NOT_FOUND;
		}
	}
	if (func_data->func_iface != 0xFF)
	{
		func_data->func_iface   = 0xFF;
		func_data->func_alt_set = 0xFF;
	}
	
	func_data->enabled = false;

	if (func_data->func_bulk_ep[MIDI_TRANS_OUT] != 0xFF)
	{
		usb_d_ep_deinit(func_data->func_bulk_ep[MIDI_TRANS_OUT]);
		func_data->func_bulk_ep[MIDI_TRANS_OUT] = 0xFF;
	}

	if (func_data->func_bulk_ep[MIDI_TRANS_IN] != 0xFF)
	{
		usb_d_ep_deinit(func_data->func_bulk_ep[MIDI_TRANS_IN]);
		func_data->func_bulk_ep[MIDI_TRANS_IN] = 0xFF;
	}
	
	_MIDIdf_funcd.enabled = false;
	return ERR_NONE;
}

/**
* \brief Get MIDI Alternate Setting
* \param[in] drv	-	Pointer to USB device function driver
* \param[in] desc	-	Pointer to USB device descriptor
* \return Currently selected alternate setting.
*/
static int32_t MIDIdf_get_iface(struct usbdf_driver *drv, struct usb_req *req)
{
	(void)drv;

	if (req->wIndex == _MIDIdf_funcd.func_iface)
	{
		return _MIDIdf_funcd.func_alt_set;
	}
	else
	{
		return ERR_NOT_FOUND;
	}
}

/**
* \brief MIDI Control Function
* \param[in] drv	-	Pointer to USB device function driver
* \param[in] ctrl	-	USB device general function control type
* \param[in] param	-	Parameter pointer
* \return Operation status.
*/
static int32_t MIDIdf_ctrl(struct usbdf_driver *drv, enum usbdf_control ctrl, void *param)
{
	switch (ctrl)
	{
		case USBDF_ENABLE:
		return MIDIdf_enable(drv, (struct usbd_descriptors *)param);
		
		case USBDF_DISABLE:
		return MIDIdf_disable(drv, (struct usbd_descriptors *)param);

		case USBDF_GET_IFACE:
		return MIDIdf_get_iface(drv, (struct usb_req *)param);

		default:
		return ERR_INVALID_ARG;
	}
}

/**
* \brief Process the MIDI class request
* \param[in] ep		-	Endpoint address.
* \param[in] req	-	Pointer to the request.
* \return Operation status.
*/
static int32_t MIDIdf_req(uint8_t ep, struct usb_req *req, enum usb_ctrl_stage stage)
{
	(void)ep;

	endp = ep;
	bmRequestType = req->bmRequestType;
	bRequest = req->bRequest;
	wIndex = req->wIndex;
	vValue = req->wValue;
	test = 1;

	
	//if (0x01 != ((req->bmRequestType >> 5) & 0x03)) /* MIDI specific request */
	//{ 
		//return ERR_NOT_FOUND;
	//}

	if (req->wIndex == _MIDIdf_funcd.func_iface && 0 == req->bRequest)
	{
		if (USB_EP_DIR_IN == (0x80 & req->bmRequestType))
		{
			if (NULL != MIDI_ctrl_wr && USB_SETUP_STAGE == stage)
			{
				MIDI_ctrl_wr(req->wLength);
			}
			return ERR_NONE;
		}
		else
		{
			if (0 != req->wLength && NULL != MIDI_ctrl_rd && USB_SETUP_STAGE == stage)
			{
				MIDI_ctrl_rd(req->wLength);
			}
			return ERR_NONE;
		}
	}
	else
	{
		return ERR_NOT_FOUND;
	}
}

/** USB Device MIDI Handler Struct */
static struct usbdc_handler MIDIdf_req_h = {NULL, (FUNC_PTR)MIDIdf_req};

/**
* \brief Initialize the USB MIDI Function Driver
*/
int32_t MIDIdf_init(void)
{
	if (usbdc_get_state() > USBD_S_POWER) {
		return ERR_DENIED;
	}

	_MIDIdf.ctrl      = MIDIdf_ctrl;
	_MIDIdf.func_data = &_MIDIdf_funcd;

	usbdc_register_function(&_MIDIdf);
	usbdc_register_handler(USBDC_HDL_REQ, &MIDIdf_req_h);

	return ERR_NONE;
}

/**
* \brief Deinitialize the USB MIDI Function Driver
*/
int32_t MIDIdf_deinit(void)
{
	if (usbdc_get_state() > USBD_S_POWER) {
		return ERR_DENIED;
	}

	_MIDIdf.ctrl      = NULL;
	_MIDIdf.func_data = NULL;

	usbdc_unregister_function(&_MIDIdf);
	usbdc_unregister_handler(USBDC_HDL_REQ, &MIDIdf_req_h);
	return ERR_NONE;
}

/**
* \brief Check whether MIDI Function is enabled
*/
bool MIDIdf_is_enabled(void)
{
	return _MIDIdf_funcd.enabled;
}

/**
* \brief USB MIDI Function Read Data
*/
int32_t MIDIdf_read(enum usb_ep_type type, uint8_t *buf, uint32_t size)
{
	uint8_t ep;

	if (!MIDIdf_is_enabled()) {
		return ERR_DENIED;
	}

	switch (type) {
		case USB_EP_TYPE_CONTROL:
		ep = 0;
		break;
		case USB_EP_TYPE_BULK:
		ep = _MIDIdf_funcd.func_bulk_ep[MIDI_TRANS_OUT];
		break;
		default:
		return ERR_INVALID_ARG;
	}

	return usbdc_xfer(ep, buf, size, false);
}

/**
* \brief USB MIDI Function Write Data
*/
int32_t MIDIdf_write(enum usb_ep_type type, uint8_t *buf, uint32_t size)
{
	uint8_t ep;

	if (!MIDIdf_is_enabled()) {
		return ERR_DENIED;
	}

	switch (type) {
		case USB_EP_TYPE_CONTROL:
		ep = 0;
		break;
		case USB_EP_TYPE_BULK:
		ep = _MIDIdf_funcd.func_bulk_ep[MIDI_TRANS_IN];
		break;
		default:
		return ERR_INVALID_ARG;
	}

	return usbdc_xfer(ep, buf, size, false);
}

/**
* \brief USB MIDI Function Register Callback
*/
int32_t MIDIdf_register_callback(enum MIDIdf_cb_type cb_type, FUNC_PTR func)
{
	switch (cb_type) {
		case MIDIDF_CB_CTRL_READ_REQ:
		MIDI_ctrl_rd = (MIDI_ctrl_cb_t)func;
		break;
		case MIDIDF_CB_CTRL_WRITE_REQ:
		MIDI_ctrl_wr = (MIDI_ctrl_cb_t)func;
		break;
		case MIDIDF_CB_BULK_READ:
		MIDI_bulk_rd = func;
		break;
		case MIDIDF_CB_BULK_WRITE:
		MIDI_bulk_wr = func;
		break;
		default:
		return ERR_INVALID_ARG;
	}

	return ERR_NONE;
}

/**
* \brief Return version
*/
uint32_t MIDIdf_get_version(void)
{
	return MIDI_VERSION;
}

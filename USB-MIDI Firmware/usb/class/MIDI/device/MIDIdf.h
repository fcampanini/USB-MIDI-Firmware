/**
*Copyright 2021 Filippo Campanini
*
*CC BY-NC-SA License
*
*http://creativecommons.org/licenses/by-nc-sa/4.0/
*
*NOTICES
*THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
*INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
*IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
*WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*/


#ifndef MIDIDF_H_
#define MIDIDF_H_

#include "usbdc.h"
#include "usb_protocol_MIDI.h"

/** MIDI Callback Type */
enum MIDIdf_cb_type {
	/* Ctrl Endpoint Type Transfer */
	MIDIDF_CB_CTRL_READ_REQ,
	MIDIDF_CB_CTRL_WRITE_REQ,
	/* Bulk Endpoint Type Transfer */
	MIDIDF_CB_BULK_READ,
	MIDIDF_CB_BULK_WRITE
};

/* USB Device MIDI Ctrl Endpoint Transfer Callback */
typedef void (*MIDI_ctrl_cb_t)(uint16_t);

/**
 * \brief Initialize the USB MIDI Function Driver
 * \return Operation status.
 */
int32_t MIDIdf_init(void);

/**
 * \brief Deinitialize the USB MIDI Function Driver
 * \return Operation status.
 */
int32_t MIDIdf_deinit(void);

/**
 * \brief Check whether MIDI Function is enabled
 * \return Operation status.
 * \return true if MIDI Function is enabled
 * \return false if MIDI Function is disabled
 */
bool MIDIdf_is_enabled(void);

/**
 * \brief USB MIDI Function Read Data
 * \param[in] type	-	Endpoint transfer types
 * \param[in] buf	-	Pointer to the buffer which receives data
 * \param[in] size	-	the size of data to be received
 * \return Operation status.
 */
int32_t MIDIdf_read(enum usb_ep_type type, uint8_t *buf, uint32_t size);

/**
 * \brief USB MIDI Function Write Data
 * \param[in] type	-	Endpoint transfer types
 * \param[in] buf	-	Pointer to the buffer which stores data
 * \param[in] size	-	the size of data to be sent
 * \return Operation status.
 */
int32_t MIDIdf_write(enum usb_ep_type type, uint8_t *buf, uint32_t size);

/**
 * \brief USB MIDI Function Register Callback
 * \param[in] cb_type	-	Callback type of Vendor Function
 * \param[in] func		-	Pointer to callback function
 * \return Operation status.
 */
int32_t MIDIdf_register_callback(enum MIDIdf_cb_type cb_type, FUNC_PTR func);

/**
 * \brief Return version
 */
uint32_t MIDIdf_get_version(void);

#endif /* MIDIDF_H_ */
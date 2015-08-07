/*
* PLabServerStrings
* Version 0.1, August, 2015
* Internal part of PLabFileServer. Holds all strings stored in PROGMEM.
*
* Created by Inge Edward Halsaunet, 2015
* Released into the public domain
*/

/*
Definitions of strings:
 Definitions of request fields
 Definitions of MIME types.
 Two arrays that contain (1) suffixes and (2) MIME type of given suffix.
 EXACT ORDERING of the two arrays IS REQUIRED!
 */

#ifndef PLAB_SERVER_STRINGS_H
#define PLAB_SERVER_STRINGS_H

#include <avr/pgmspace.h>

#ifndef PLAB_NO_MIMES

// 4 characters + '\0'
#define PLAB_MIME_SUFFIX_BUF_MAX 5

// Basic web mime
const char plabHtmMIMESuf[] PROGMEM  = "htm";
const char plabHtmMIMEType[] PROGMEM = "text/html";

const char plabHtmlMIMESuf[] PROGMEM = "html";
#endif //ndef PLAB_NO_MIMES
// We will need this mime type in error codes
const char plabHtmlMIMEType[] PROGMEM = "text/html";
#ifndef PLAB_NO_MIMES

const char plabXmlMIMESuf[] PROGMEM = "xml";
const char plabXmlMIMEType[] PROGMEM = "application/xml";

const char plabCssMIMESuf[] PROGMEM = "css";
const char plabCssMIMEType[] PROGMEM = "text/css";

const char plabJsMIMESuf[] PROGMEM = "js";
const char plabJsMIMEType[] PROGMEM = "application/javascript";

const char plabJsonMIMESuf[] PROGMEM = "json";
const char plabJsonMIMEType[] PROGMEM = "application/json";

// Basic image mime
const char plabJpgMIMESuf[] PROGMEM = "jpg";
const char plabJpgMIMEType[] PROGMEM = "image/jpeg";

const char plabJpegMIMESuf[] PROGMEM = "jpeg";
const char plabJpegMIMEType[] PROGMEM = "image/jpeg";

const char plabPngMIMESuf[] PROGMEM = "png";
const char plabPngMIMEType[] PROGMEM = "image/png";

const char plabGifMIMESuf[] PROGMEM = "gif";
const char plabGifMIMEType[] PROGMEM = "image/gif";

const char plabSvgMIMESuf[] PROGMEM = "svg";
const char plabSvgMIMEType[] PROGMEM = "image/svg+xml";

#else
#define PLAB_MIME_SUFFIX_BUF_MAX 0
#define PLAB_MIME_TYPE_BUF_MAX 0
#endif // else ndef PLAB_NO_MIMES

const char* const plabMIMESuffixTable[] PROGMEM = {

#ifndef PLAB_NO_MIMES

	// Basic web mime
	plabHtmMIMESuf,
	plabHtmlMIMESuf,
	plabXmlMIMESuf,
	plabCssMIMESuf,
	plabJsMIMESuf,
	plabJsonMIMESuf,
	// Basic image mime
	plabJpgMIMESuf,
	plabJpegMIMESuf,
	plabPngMIMESuf,
	plabGifMIMESuf,
	plabSvgMIMESuf

#endif // ndef PLAB_NO_MIMES

};

const char* const plabMIMETypeTable[] PROGMEM = {

#ifndef PLAB_NO_MIMES
	// Basic web mime
	plabHtmMIMEType,
	plabHtmlMIMEType,
	plabXmlMIMEType,
	plabCssMIMEType,
	plabJsMIMEType,
	plabJsonMIMEType,
	// Basic image mime
	plabJpgMIMEType,
	plabJpegMIMEType,
	plabPngMIMEType,
	plabGifMIMEType,
	plabSvgMIMEType
#endif // ndef PLAB_NO_MIMES

};

// user feedback strings
const char plabRequest[] PROGMEM = "REQUEST:";
const char plabResponse[] PROGMEM = "RESPONSE:";
// Response header strings
const char plabHeaderHttp[] PROGMEM = "HTTP/1.1 ";
const char plab200OK[] PROGMEM = "200 OK";
const char plabContentType[] PROGMEM = "Content-Type: ";
const char plabConnectionClose[] PROGMEM = "Connection: close";
// Error code strings. Note: all have three digits, a space and message
const char plab400BadRequest[] PROGMEM = "400 Bad Request";
const char plab404NotFound[] PROGMEM = "404 Not Found";
const char plab405MethodNotAllowed[] PROGMEM = "405 Method Not Allowed";
const char plab414RequestUriTooLarge[] PROGMEM = "414 Request-URI Too Large";
const char plab500InternalServerError[] PROGMEM = "500 Internal Server Error";
const char plab501NotImplemented[] PROGMEM = "501 Not Implemented";
const char plab505HttpVersionNotSupported[] PROGMEM = "505 HTTP Version not supported";
// Standard error html, excluding code and reason
const char plabHtml1[] PROGMEM = "<!DOCTYPE html>\r\n<html><head><title>";
const char plabHtml2[] PROGMEM = "</title></head><body><h1>";
const char plabHtml3[] PROGMEM = "</h1></body></html>";

#endif // ndef PLAB_MIME_TYPES_H

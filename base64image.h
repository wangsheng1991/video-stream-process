#ifndef BASE64IMAGE_H
#define BASE64IMAGE_H

#include <opencv/highgui.h>
#include <string>
using std::string;
class Base64Image
{
public:
    Base64Image();
    static int encodeImage(const IplImage *img,string &encodedImg);
    static int decodeImage(const string &decodeImg,char *imgData);
};

#endif // BASE64IMAGE_H

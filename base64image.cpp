#include "base64image.h"
#include "base64.h"
Base64Image::Base64Image()
{




}

int Base64Image::encodeImage(const IplImage *img, std::string &encodedImg)
{
    if(NULL==img||0>=img->imageSize)
    {
        return -1;
    }
    encodedImg.resize(img->imageSize);
   if(!Base64::Encode(img->imageData,img->imageSize,&encodedImg[0],encodedImg.size()))
   {
       return -1;
   }

   return 0;

}
int Base64Image::decodeImage(const std::string &decodeImg, char *imgData)
{



    return 0;
}

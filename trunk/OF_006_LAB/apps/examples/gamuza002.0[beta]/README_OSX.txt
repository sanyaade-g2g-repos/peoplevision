///////////////////////GAMUZA PROJECT

 - you need OF 0.06 FAT [http://www.openframeworks.cc] installed and running with xcode environment

 - open ofxCvColorImage.rtf file, copy his content and replace the function in ofxcvColorImage.cpp file of ofxOpenCv addon (line 217)
   this is a fix for the convertToGrayscalePlanarImages(ofxCvGrayscaleImage& red, ofxCvGrayscaleImage& green, ofxCvGrayscaleImage& blue) function

 - just run Gamuza.xcodeproj like an OF example


///////////////////////GAMUZA APPLICATION

 - just download the gamuzaApp.zip from the web [http://www.gamuza.org], extract the files wherever you want, launch Gamuza.app
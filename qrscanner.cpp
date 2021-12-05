#include "qrscanner.h"

QRScanner::QRScanner()
{

}

/*
void QRScanner::decode(Mat &src, vector<QRScanner::decoded_object> decoded_objs)
{

      // Create zbar scanner
      ImageScanner scanner;

      // Configure scanner
      scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

      // Convert image to grayscale
      Mat imGray;
      cvtColor(src, imGray,CV_BGR2GRAY);

      // Wrap image data in a zbar image
      Image image(src.cols, src.rows, "Y800", (uchar *)imGray.data, src.cols * src.rows);

      // Scan the image for barcodes and QRCodes
      int n = scanner.scan(image);

      // Print results
      for(Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
      {
        decoded_object obj;

        obj. type = symbol->get_type_name();
        obj.data = symbol->get_data();

        // Print type and data
        cout << "Type : " << obj.type << endl;
        cout << "Data : " << obj.data << endl << endl;

        // Obtain location
        for(int i = 0; i< symbol->get_location_size(); i++)
        {
          obj.location.push_back(Point(symbol->get_location_x(i),symbol->get_location_y(i)));
        }

        decoded_objs.push_back(obj);
      }


}
void QRScanner::display(Mat &im, vector<decoded_object>&decodedObjects)
{
  // Loop over all decoded objects
  for(int i = 0; i < decodedObjects.size(); i++)
  {
    vector<Point> points = decodedObjects[i].location;
    vector<Point> hull;

    // If the points do not form a quad, find convex hull
    if(points.size() > 4)
      convexHull(points, hull);
    else
      hull = points;

    // Number of points in the convex hull
    int n = hull.size();

    for(int j = 0; j < n; j++)
    {
      line(im, hull[j], hull[ (j+1) % n], Scalar(255,0,0), 3);
    }

  }

  // Display results
  imshow("Results", im);
  waitKey(0);

}
*/

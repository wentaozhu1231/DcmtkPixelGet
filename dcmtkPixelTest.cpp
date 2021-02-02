// wentao zhu 1/17/2021
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string>

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimgle/dcmimage.h"

using namespace std;

int main(int argc, char* argv[]) {
  try {
    const std::string file_path =
        "C:\\RI.1.2.246.352.81.3.3417524247.25685.18122.148.193--500mu.dcm";
    if (file_path.length() == 0) exit(EXIT_FAILURE);

    const DicomImage dcm_image(file_path.c_str());

    const auto rows = dcm_image.getHeight();
    const auto cols = dcm_image.getWidth();

    DcmFileFormat file_format;
    const auto condition = file_format.loadFile(file_path.c_str());
    if (condition.bad()) {
      std::cerr << "cannot load the file. " << std::endl;
      return EXIT_FAILURE;
    }
    DcmDataset* data_set = file_format.getDataset();
    const E_TransferSyntax xfer = data_set->getOriginalXfer();
    // decompress data set if compressed
    data_set->chooseRepresentation(xfer, nullptr);
    // data_set->chooseRepresentation(EXS_LittleEndianExplicit, nullptr);
    DcmElement* element = nullptr;
    OFCondition result = data_set->findAndGetElement(DCM_PixelData, element);
    if (result.bad() || element == nullptr) return 1;
    unsigned short* pix_data;
    result = element->getUint16Array(pix_data);

    cv::Mat dst2(rows, cols, CV_16UC1, cv::Scalar::all(0));
    unsigned short* data = nullptr;
    for (auto i = 0; i < rows; i++) {
      // could use dst2.at<unsigned short>(i, j) = ?
      data = dst2.ptr<unsigned short>(i);
      for (auto j = 0; j < cols; j++) {
        auto temp = pix_data[i * cols + j];
        temp = temp == 65535 ? 0 : temp;
        *data++ = temp;
      }
    }
    std::vector<int> m_v;
    for (auto i = 0; i < cols; i++) {
      //std::cout << pix_data[340 * cols + i] << " ";
      m_v.emplace_back(pix_data[340 * cols + i]);
    }
    for (auto x:m_v)
    {
      std::cout << x <<" ";
    }
    std::cout << std::endl;

    cv::imshow("Test16D", dst2);
    cv::waitKey(0);
    cv::destroyAllWindows();
  } catch (exception& e) {
    std::cerr << "error" << e.what() << " ";
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
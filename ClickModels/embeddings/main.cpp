#include "MyLearner.h"
#include "day_data.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "pairspreperation.h"
//#include "embbedingbydays.h"
#include "personalizedclickmodel.h"
#include "Embedding.h"
#include "collaborative_filtering.h"
#include "mergecf_ubm.h"
#include "newmodel.h"
//#include "ubm.h"
#include <ctime>
#include <ctime>

//string out_directory = "/home/stepan/click_models_data/";
static string out_directory = "/Users/annasepliaraskaia/Desktop/work/";
//static string out_directory = "/home/anna/Рабочий стол/work/data/";



int main()
{


      //UBM ubm(out_directory, 1, 8);
      //ubm.LearnEM();
      //ubm.MyLearn();

//      collaborative_filtering cf(-1., 10, out_directory + "users", out_directory + "queries", out_directory + "data_stat/histogramms/resul_train", 10, 1000000);
//      for (int i = 0; i < 1; ++i)
//      {
//            cf.rate /= std::sqrt(i+1);
//            cf.Learn_by_several_daya(out_directory, 26, 27, true);
//            cf.rate *= std::sqrt(i+1);
//      }
//    R r(out_directory + "data_stat/histogramms/resul_train");
//    r.rate = -1;
//    for (int i = 0; i < 10; ++i)
//    {
//        std::cout << "ITERATION " << i << std::endl;
//        r.rate /= std::sqrt(i+1);
//        std::cout << "Run Learn\n";
//        r.Learn(out_directory + "data_stat/histogramms/resul_train", out_directory + "data_stat/histogramms/result_train");
//        r.rate *= std::sqrt(i+1);
//    }
    NewModel n;
    n.learn();

}

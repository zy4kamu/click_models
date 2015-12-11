#ifndef MACRO_PARAMETERS
#define MACRO_PARAMETERS

//static const string OUT_DIRECTORY = "/home/stepan/click_models_data/";
static const string OUT_DIRECTORY =  "/Users/annasepliaraskaia/Desktop/work/";
static const string DAY_DATA_FOLDER = OUT_DIRECTORY + "data_by_days/";
static const string INPUT_FOLDER = OUT_DIRECTORY + "initial_model_parameters_3/";
static const string INITIAL_MODEL_PARAMETERS_FOLDER = OUT_DIRECTORY + "initial_model_parameters_3/";
static const size_t DIMENSION = 101;
static const size_t SERP_SIZE = 10;
static const size_t FIRST_TRAINING_DAY = 1;
static const size_t LAST_TRAINING_DAY = 5;
static const double EPS = 1e-2;
static double LEARNING_RATE = -1;
static const double MIN_EMBEDDING_VALUE = 1e-10;
static const double MAX_EMBEDDING_VALUE = 1 - 1e-10;
static const string PATH_TO_DATA = OUT_DIRECTORY;

#endif // MACRO_PARAMETERS

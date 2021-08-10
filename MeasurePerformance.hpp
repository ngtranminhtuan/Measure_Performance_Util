/******************************************************************************
(C) Copyright 2020 Digital Media Professionals Inc. All rights reserved       *
                                                                              *
This source code and any compilation or derivative thereof is the sole        *
property of Digital Media Professionals Inc. and is provided pursuant to a    *
Software License Agreement. This code is the proprietary information          *
of Digital Media Professionals Inc. and is confidential in nature. Its use    *
and dissemination by any party other than Digital Media Professionals Inc. is *
strictly limited by the confidential information provisions of the            *
Agreement referenced above.                                                   *
******************************************************************************/
/*!
  * file MeasurePerformance.hpp
  * brief <>
  * date 2020-06-29
  * author trungdung <dung.vvtrung@dmprof.com>
  */
#ifndef MEASUREPERFORMANCE_HPP
#define MEASUREPERFORMANCE_HPP

#include <mutex>
#include <map>
#include <vector>
#include <chrono>
#include <list>

#define ENABLE_MEASURE_PERFORMANCE                  1

#define MS_LV0_INPUT_REPROCESS                      "INPUT_REPROCESS"
#define MS_LV0_INPUT_REPROCESS_WITH_DELAY           "INPUT_REPROCESS_WITH_DELAY"

#define MS_LV0_DFV                                  "DFV_RUN"
#define MS_LV0_DFV_POST                             "DFV_POST"
#define MS_LV0_FEED_FRAME                           "FEED_FRAME"
#define MS_LV0_COMPLETE_FRAME                       "COMPLETE_FRAME"

#define MS_LV1_CREATE_FRAME                         "CREATE_FRAME"
#define MS_LV1_TRACKING                             "TRACKING"
#define MS_LV1_TOTAL_TRACKING_FRAME                 "TRACKING_FRAME_TOTAL"
#define MS_LV1_PUBLISH_RESULT                       "PUBLISH_RESULT"

#define MS_LV2_WAIT_DEPTH                           "WAIT_DEPTH"
#define MS_LV2_CR_FRAME_JOIN_THREAD                 "JOIN_THREAD"
#define MS_LV2_CR_FRAME_ORB_EXTRACT                 "ORB_EXTRACT"
#define MS_LV2_CR_FRAME_ORB_EXTRACT_RIGHT           "ORB_EXTRACT_RIGHT"
#define MS_LV2_CR_FRAME_COMPUTE_DISPARITY           "COMPUTE_DISPARITY"
#define MS_LV2_CR_FRAME_ASSIGN_KP_TO_GRID           "ASSIGN_KP_TO_GRID"
#define MS_LV2_CR_FRAME_TOTAL                       "CREATE_FRAME_TOTAL"

#define MS_LV2_TRACKING_INITIALIZE                  "INITIALIZE_TRACKING"
#define MS_LV2_INITIALIZE_UPDATE_LOCAL_MAP          "INITIALIZE_UPDATE_LOCAL_MAP"
#define MS_LV2_TOTAL_INITIALIZE                     "INITIALIZE_TOTAL"

#define MS_LV2_TRACKING_CURRENT_FRAME               "TRACK_CURR_FRAME"
#define MS_LV2_TRACKING_UPDATE_MAP                  "TRACKING_UPDATE_LOCAL_MAP"
#define MS_LV2_TRACKING_OPTIMIZE_POSE               "TRACKING_OPTIMIZE_POSE"
#define MS_LV2_TRACKING_INSERT_KF                   "TRACKING_INSERT_KF"
#define MS_LV2_TRACKING_TIDY_UP_OBSERVATION         "TRACKING_TIDY_UP_OBSERVATON"
#define MS_LV2_TOTAL_TRACKING                       "TRACKING_TOTAL"

#define MS_LV3_TRACK_CURR_FRAME_MOTION_TRACK        "MOTION_TRACKING"
#define MS_LV3_TRACK_CURR_FRAME_BOW_TRACK           "BOW_TRACKING"
#define MS_LV3_TRACK_CURR_FRAME_ROBUST_TRACK        "ROBUST_TRACKING"
#define MS_LV3_TRACK_CURR_FRAME_RELOCALIZE          "RELOCALIZE"
#define MS_LV3_TRACK_CURR_FRAME_TOTAL               "TRACK_CURR_FRAME_TOTAL"

#define MS_LV4_MOTION_TRACK_INITIALIZE_VALUE        "MOTION_TRACKING_INITIALIZE"
#define MS_LV4_MOTION_TRACK_MATCHING_LAST_FRAME     "MOTION_TRACKING_MATCHING_LAST_FRAME"
#define MS_LV4_MOTION_TRACK_POSE_OPTIMIZE           "MOTION_TRACKING_POSE_OPTIMIZE"
#define MS_LV4_MOTION_TRACK_DISCARD_OUTLIER         "MOTION_TRACKING_DISCARD_OUTLIER"
#define MS_LV4_MOTION_TOTAL                         "MOTION_TRACKING_TOTAL"

#define MS_LV4_BOW_TRACK_COMPUTE_BOW                "BOW_TRACKING_COMPUTE"
#define MS_LV4_BOW_TRACK_MATCHING_KEY_FRAME         "BOW_TRACKING_MATCHING_KF"
#define MS_LV4_BOW_TRACK_POSE_OPTIMIZE              "BOW_TRACKING_POSE_OPTIMIZE"
#define MS_LV4_BOW_TRACK_DISCARD_OUTLIER            "BOW_TRACKING_DISCARD_OUTLIER"
#define MS_LV4_BOW_TOTAL                            "BOW_TRACKING_TOTAL"

#define MS_LV4_ROBUST_TRACK_MATCHING_KEY_FRAME      "ROBUST_TRACKING_MATCHING_KF"
#define MS_LV4_ROBUST_TRACK_POSE_OPTIMIZE           "ROBUST_TRACKING_POSE_OPTIMIZE"
#define MS_LV4_ROBUST_TRACK_DISCARD_OUTLIER         "ROBUST_TRACKING_DISCARD_OUTLIER"
#define MS_LV4_ROBUST_TOTAL                         "ROBUST_TRACKING_TOTAL"

#define MS_LV1_LOCAL_BUNDLE_ADJUSTMENT              "LOCAL_BUNDLE"
#define MS_LV1_LOCAL_MAPPING_TOTAL                  "LOCAL_MAPPING_TOTAL"

#define MS_LV2_DFV_FILL_TENSOR                      "DFV_FILL_TENSOR"
#define MS_LV2_DFV_SESSION_RUN                      "DFV_SESSION_RUN"
#define MS_LV2_DFV_GET_TENSOR_OUTPUT                "DFV_GET_TENSOR_OUTPUT"

// ================= Performace with GPS ===================
// gps_initializer module (in global_optimization thread)
#define MS_LV2_P0_FINETUNE_CAL "P0_FINETUNE_CAL"
#define MS_LV2_P0_FINETUNE_UPDATE_MAP "P0_FINETUNE_UPDATE_MAP"
#define MS_LV2_SCALE_INIT_CAL "CALCULATE_SCALE_INIT"
#define MS_LV2_SCALE_INIT_UPDATE_MAP "SCALE_INIT_UPDATE_MAP"
#define MS_LV2_POSE_INIT_CAL "CALCULATE_POSE_INIT"
#define MS_LV2_POSE_INIT_UPDATE_MAP "POSE_INIT_UPDATE_MAP"

// BLBA module (in blba thread)
#define MS_LV2_BLBA_OPTIMIZE "OPTIMIZE"
#define MS_LV2_BLBA_PRE_UPDATE "PRE_UPDATE_MAP" //re update map (blba maybe wait tracking and mapping)
#define MS_LV2_BLBA_APPLY_UPDATE "APPLY_UPDATE_MAP" //apply update map (tracking & mapping wait blba)

// Performance path planning
#define MS_LV2_LOCAL_PATH_PLANNING "LOCAL_PATH_PLANNING"
#define MS_LV2_GLOBAL_PATH_PLANNING "GLOBAL_PATH_PLANNING"

class MeasurePerformance
{
public:
    /* measue performance in milisecond */
    MeasurePerformance();
    static MeasurePerformance *instance();

    void measure(std::string id, double perform);
    void measure(std::string id,
                 std::chrono::system_clock::time_point tp1,
                 std::chrono::system_clock::time_point tp2);

    static std::chrono::system_clock::time_point now();
    static double duration(std::chrono::system_clock::time_point tp1,
                                        std::chrono::system_clock::time_point tp2);


    void save(std::string file);
    void save(std::string file, std::vector<std::string> properties);
    void summary(std::string mode);
protected:
    bool isDirectoryExists(const char *path);
    void createDir(const char *dir);
private:
    static MeasurePerformance* m_instance;
    static std::mutex *m_mutex;
    std::map<std::string, std::vector<double>> m_measure;
    std::string m_output_dir;

};

#endif // MEASUREPERFORMANCE_HPP

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
  * file MeasurePerformance.cpp
  * brief <>
  * date 2020-06-29
  * author trungdung <dung.vvtrung@dmprof.com>
  */
#include "MeasurePerformance.hpp"

#include <fstream>
#include <sys/stat.h>
#include <algorithm>

#include "spdlog/spdlog.h"


MeasurePerformance* MeasurePerformance::m_instance = nullptr;
std::mutex *MeasurePerformance::m_mutex = new std::mutex();

MeasurePerformance::MeasurePerformance()
{
    m_output_dir = "../eval_log/measure_output";
    createDir(m_output_dir.c_str());
}

MeasurePerformance *MeasurePerformance::instance()
{
#ifdef ENABLE_MEASURE_PERFORMANCE
    std::lock_guard<std::mutex> lock(*m_mutex);
    if(!m_instance)
    {
        m_instance = new MeasurePerformance();
    }
#endif
    return m_instance;

}

bool MeasurePerformance::isDirectoryExists(const char *path)
{
    struct stat stats;

    stat(path, &stats);

    // Check for file existence
    if (S_ISDIR(stats.st_mode))
        return true;

    return false;
}

void MeasurePerformance::createDir(const char *dir)
{
    if(!isDirectoryExists(dir))
    {
        mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
}


void MeasurePerformance::measure(std::string id, double perform)
{
#ifdef ENABLE_MEASURE_PERFORMANCE
    std::lock_guard<std::mutex> lock(*m_mutex);
    std::map<std::string, std::vector<double>>::iterator eleIt= m_measure.find(id);
    if(eleIt != m_measure.end())
    {
        eleIt->second.push_back(perform);
    }
    else
    {
        std::vector<double> data;
        data.push_back(perform);
        m_measure[id] = data;
    }
#endif
}

void MeasurePerformance::measure(std::string id,
                                 std::chrono::system_clock::time_point tp1,
                                 std::chrono::system_clock::time_point tp2)
{
#ifdef ENABLE_MEASURE_PERFORMANCE
    std::lock_guard<std::mutex> lock(*m_mutex);
    double perform = duration(tp1, tp2);
    std::map<std::string, std::vector<double>>::iterator eleIt= m_measure.find(id);
    if(eleIt != m_measure.end())
    {
        eleIt->second.push_back(perform);
    }
    else
    {
        std::vector<double> data;
        data.push_back(perform);
        m_measure[id] = data;
    }
#endif
}

std::chrono::system_clock::time_point MeasurePerformance::now()
{
    return std::chrono::system_clock::now();
}

double MeasurePerformance::duration(std::chrono::system_clock::time_point tp1,
                                    std::chrono::system_clock::time_point tp2)
{
    double time = 0.0;
#ifdef ENABLE_MEASURE_PERFORMANCE
    time = std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count();
#endif
    return time;
}

void MeasurePerformance::save(std::string file)
{
#ifdef ENABLE_MEASURE_PERFORMANCE
    std::lock_guard<std::mutex> lock(*m_mutex);
    std::ofstream ofs(m_output_dir + "/" + file, std::ios::out);
    std::vector<std::string> export_datas;
    std::string header;
    int row = 0;

    for(auto it=m_measure.begin(); it!=m_measure.end(); it++)
    {
        if(row<it->second.size())
        {
            row = it->second.size();
        }/* get lagest row */
        header = header + it->first + ",";
    }
    export_datas.resize(row);

    if (ofs.is_open())
    {
        for(int y=0; y<row; y++)
        {
            for(auto it=m_measure.begin(); it!=m_measure.end(); it++)
            {
                if(it->second.size()>y)
                {
                    export_datas[y] = export_datas[y] + std::to_string(it->second[y]) + ",";
                }
                else
                {
                    export_datas[y] = export_datas[y] + ",";
                }/* skip cell */

            }
        }

        ofs << header << std::endl;
        for (const auto line : export_datas)
        {
            ofs << line << std::endl;
        }

        ofs.close();
    }
#endif
}

void MeasurePerformance::save(std::string file, std::vector<std::string> properties)
{
#ifdef ENABLE_MEASURE_PERFORMANCE
    std::lock_guard<std::mutex> lock(*m_mutex);
    std::ofstream ofs(m_output_dir + "/" + file, std::ios::out);
    std::vector<std::string> export_datas;
    std::string header;
    int row = 0;
    std::map<std::string, std::vector<double>> measureFiltered;
    for(auto it = m_measure.begin(); it!=m_measure.end(); it++)
    {
         std::vector<std::string>::iterator exist =
                 std::find(properties.begin(), properties.end(), it->first);
        if(exist == properties.end())
        {
            continue;
        }
        else
        {
            measureFiltered.insert(std::pair<std::string, std::vector<double>>(it->first, it->second));
            if(row<it->second.size())
            {
                row = it->second.size();
            }
            header = header + it->first + ",";
        }
    }

    export_datas.resize(row);

    if (ofs.is_open())
    {
        for(int y=0; y<row; y++)
        {
            for(auto it=measureFiltered.begin(); it!=measureFiltered.end(); it++)
            {
                if(it->second.size()>y)
                {
                    export_datas[y] = export_datas[y] + std::to_string(it->second[y]) + ",";
                }
                else
                {
                    export_datas[y] = export_datas[y] + ",";
                }/* skip cell */

            }
        }

        ofs << header << std::endl;
        for (const auto line : export_datas)
        {
            ofs << line << std::endl;
        }

        ofs.close();
    }
#endif
}

void MeasurePerformance::summary(std::string mode)
{
#ifdef ENABLE_MEASURE_PERFORMANCE
    this->save("performance_"+mode+".csv");

    std::vector<std::string> properties_app = {MS_LV0_INPUT_REPROCESS,
                                               MS_LV0_INPUT_REPROCESS_WITH_DELAY,
                                               MS_LV0_FEED_FRAME,
                                               MS_LV0_COMPLETE_FRAME
                                              };
    this->save("performance_app.csv", properties_app);

    std::vector<std::string> properties_tracking = {MS_LV1_CREATE_FRAME,
                                                    MS_LV1_TRACKING,
                                                    MS_LV1_TOTAL_TRACKING_FRAME,
                                                    MS_LV1_PUBLISH_RESULT
                                                   };
    this->save("performance_tracking .csv", properties_tracking);

    std::vector<std::string> properties_create_frame = {
                                                        MS_LV2_CR_FRAME_ORB_EXTRACT,
                                                        MS_LV2_CR_FRAME_ORB_EXTRACT_RIGHT,
                                                        MS_LV2_CR_FRAME_COMPUTE_DISPARITY,
                                                        MS_LV2_CR_FRAME_ASSIGN_KP_TO_GRID,
                                                        MS_LV2_CR_FRAME_TOTAL,
                                                        MS_LV2_CR_FRAME_JOIN_THREAD,
                                                        MS_LV2_WAIT_DEPTH,
                                                       };
    this->save("performance_create_frame.csv", properties_create_frame);

    std::vector<std::string> properties_init_tracking = {MS_LV2_TRACKING_INITIALIZE,
                                                         MS_LV2_INITIALIZE_UPDATE_LOCAL_MAP,
                                                         MS_LV2_TOTAL_INITIALIZE};
    this->save("performance_init_tracking.csv", properties_init_tracking);

    std::vector<std::string> properties_track = {MS_LV2_TRACKING_CURRENT_FRAME,
                                                 MS_LV2_TRACKING_UPDATE_MAP,
                                                 MS_LV2_TRACKING_OPTIMIZE_POSE,
                                                 MS_LV2_TRACKING_INSERT_KF,
                                                 MS_LV2_TRACKING_TIDY_UP_OBSERVATION,
                                                 MS_LV2_TOTAL_TRACKING
                                                };
    this->save("performance_track.csv", properties_track);

    std::vector<std::string> properties_track_curr_frame = {MS_LV3_TRACK_CURR_FRAME_MOTION_TRACK,
                                                            MS_LV3_TRACK_CURR_FRAME_BOW_TRACK,
                                                            MS_LV3_TRACK_CURR_FRAME_ROBUST_TRACK,
                                                            MS_LV3_TRACK_CURR_FRAME_RELOCALIZE,
                                                            MS_LV3_TRACK_CURR_FRAME_TOTAL
                                                           };
    this->save("performance_track_curr_frame.csv", properties_track_curr_frame);

    std::vector<std::string> properties_track_motion = {MS_LV4_MOTION_TRACK_INITIALIZE_VALUE,
                                                        MS_LV4_MOTION_TRACK_MATCHING_LAST_FRAME,
                                                        MS_LV4_MOTION_TRACK_POSE_OPTIMIZE,
                                                        MS_LV4_MOTION_TRACK_DISCARD_OUTLIER,
                                                        MS_LV4_MOTION_TOTAL
                                                       };
    this->save("performance_track_motion.csv", properties_track_motion);

    std::vector<std::string> properties_track_bow = {MS_LV4_BOW_TRACK_COMPUTE_BOW,
                                                     MS_LV4_BOW_TRACK_MATCHING_KEY_FRAME,
                                                     MS_LV4_BOW_TRACK_POSE_OPTIMIZE,
                                                     MS_LV4_BOW_TRACK_DISCARD_OUTLIER,
                                                     MS_LV4_BOW_TOTAL
                                                    };
    this->save("performance_track_bow.csv", properties_track_bow);

    std::vector<std::string> properties_track_robust = {MS_LV4_ROBUST_TRACK_MATCHING_KEY_FRAME,
                                                        MS_LV4_ROBUST_TRACK_POSE_OPTIMIZE  ,
                                                        MS_LV4_ROBUST_TRACK_DISCARD_OUTLIER ,
                                                        MS_LV4_ROBUST_TOTAL
                                                       };
    this->save("performance_track_robust.csv", properties_track_robust);

    std::vector<std::string> properties_local_mapping = {MS_LV1_LOCAL_BUNDLE_ADJUSTMENT,
                                                        MS_LV1_LOCAL_MAPPING_TOTAL
                                                       };
    this->save("performance_local_mapping.csv", properties_local_mapping);

    std::vector<std::string> properties_dfv = {MS_LV0_DFV,
                                               MS_LV0_DFV_POST,
                                               MS_LV2_DFV_FILL_TENSOR,
                                               MS_LV2_DFV_SESSION_RUN,
                                               MS_LV2_DFV_GET_TENSOR_OUTPUT
                                              };
    this->save("performance_dfv.csv", properties_dfv);

    std::vector<std::string> properties_path_planning = {MS_LV2_GLOBAL_PATH_PLANNING,
                                                        MS_LV2_LOCAL_PATH_PLANNING
                                                        };
    this->save("performance_path_planning.csv", properties_path_planning);

    spdlog::info("Performance exported to {}", m_output_dir.c_str());
#endif
}

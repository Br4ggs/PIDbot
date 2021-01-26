#ifndef ROBOTGUIDE_OBSTACLEDETECTION_H
#define ROBOTGUIDE_OBSTACLEDETECTION_H

class ObstacleDetection
{
public:
    ObstacleDetection() = default;
    ObstacleDetection(const ObstacleDetection& other) = delete;
    ObstacleDetection& operator=(const ObstacleDetection&) = delete;
    ~ObstacleDetection() = default;

    bool ObstacleDetected();
};

#endif
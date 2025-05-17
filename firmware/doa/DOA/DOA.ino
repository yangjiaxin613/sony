#include <math.h>
#include <fft/FFT.h>

#define MIC_DISTANCE_CM 5.0f     // 麦克风间距（厘米）
#define SOUND_SPEED_CM_S 34300.0f // 声速（cm/s）

float calculateDOA(int16_t* leftMic, int16_t* rightMic, int length) {
    float delay = 0.0f;
    float maxCorr = -1.0f;

    // 计算两个声道之间的时延（简化起见）
    for (int d = -10; d <= 10; d++) {
        float corr = 0.0f;
        for (int i = 0; i < length - abs(d); i++) {
            int j = i + d;
            if (j >= 0 && j < length) {
                corr += leftMic[i] * rightMic[j];
            }
        }
        if (corr > maxCorr) {
            maxCorr = corr;
            delay = d;
        }
    }

    // 转换为角度
    float angleRad = asin((delay * MIC_DISTANCE_CM) / (SOUND_SPEED_CM_S * 1000.0f / SAMPLE_RATE));
    return angleRad * 180.0f / M_PI;
}

void loopDOA(int16_t* buffer, size_t length) {
    int halfLength = length / 2;
    int16_t* left = &buffer[0];
    int16_t* right = &buffer[halfLength];

    float angle = calculateDOA(left, right, halfLength);
    Serial.print("声源方向估计角度: ");
    Serial.print(angle);
    Serial.println("°");
}
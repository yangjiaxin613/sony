#include "kws_model.h"  // 假设你已部署好模型头文件

KWSModel kws;

void setupKWS() {
    if (!kws.begin()) {
        Serial.println("KWS模型加载失败");
        while (1);
    }
}

void loopKWS(int16_t* buffer, size_t length) {
    const char* keyword = kws.run(buffer, length);
    if (keyword != nullptr) {
        Serial.print("识别到关键词: ");
        Serial.println(keyword);
    }
}
void setup() {
    Serial.begin(115200);

    setupAudioCapture(); // 初始化音频采集
    setupKWS();           // 加载关键词识别模型
}

void loop() {
    loopAudioCapture(); // 录音并存入缓冲区
    loopKWS(audioBuffer, BUFFER_SIZE); // 检测关键词
    loopDOA(audioBuffer, BUFFER_SIZE); // 判断声源方向
}
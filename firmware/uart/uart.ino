#include <Arduino.h>
#include <audio/audio.h>
#include <dsp/wav_writer.h>

// 配置参数
#define SAMPLE_RATE     16000
#define CHANNEL_COUNT   2       // 双麦克风输入
#define BUFFER_SIZE     1024

// 全局变量
bool isRecording = false;
int16_t audioBuffer[BUFFER_SIZE * CHANNEL_COUNT];

AudioClass audio;
WAVWriter wavWriter;

// 声明函数
void processCommand(String &cmd);
void startAudioProcessing();
void stopAudioProcessing();

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);  // 等待串口连接

    Serial.println("🎧 Spresense 语音控制系统启动");
    Serial.println("输入 'help' 查看可用命令");

    // 初始化音频系统
    if (!audio.begin()) {
        Serial.println("❌ 音频初始化失败");
        while (1); // 停止程序
    }
    audio.setSampleRate(SAMPLE_RATE);
    audio.setChannelCount(CHANNEL_COUNT);
}

void loop() {
    // 检查是否有串口输入
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();  // 去除前后空格
        processCommand(command);
    }

    // 如果正在录音，持续读取音频数据
    if (isRecording) {
        int bytesRead = audio.read(audioBuffer, BUFFER_SIZE);
        if (bytesRead > 0) {
            // 这里可以插入 KWS 或 DOA 处理逻辑
            // 示例：loopKWS(audioBuffer, bytesRead / sizeof(int16_t));
            // 示例：loopDOA(audioBuffer, bytesRead / sizeof(int16_t));

            Serial.print("🎤 录音中... ");
            Serial.print(bytesRead / 2);
            Serial.println(" 字节");
        }
    }
}

// 处理用户输入的命令
void processCommand(String &cmd) {
    if (cmd.equalsIgnoreCase("help")) {
        Serial.println("可用命令:");
        Serial.println("  start   - 开始录音");
        Serial.println("  stop    - 停止录音");
        Serial.println("  help    - 显示帮助信息");
    } else if (cmd.equalsIgnoreCase("start")) {
        startAudioProcessing();
    } else if (cmd.equalsIgnoreCase("stop")) {
        stopAudioProcessing();
    } else {
        Serial.println("⚠️ 未知命令，请输入 'help' 查看可用命令");
    }
}

// 开始录音
void startAudioProcessing() {
    if (!isRecording) {
        audio.startRecording();
        isRecording = true;
        Serial.println("🟢 开始录音");
    } else {
        Serial.println("⚠️ 已在录音中");
    }
}

// 停止录音
void stopAudioProcessing() {
    if (isRecording) {
        audio.stopRecording();
        isRecording = false;
        Serial.println("🛑 停止录音");
    } else {
        Serial.println("⚠️ 当前未录音");
    }
}
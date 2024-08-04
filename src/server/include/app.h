﻿#ifndef SERVER_APP_H
#define SERVER_APP_H

#include "define.h"
#include "common/singleton.h"

namespace server {
class App final: public common::Singleton<App> {
 public:
    explicit App(Token);

    ~App() override = default;

    /*
    * 初始化
    * @return ErrCode
    */
    ErrCode Init();

    /*
    * 启动
    * @return ErrCode
    */
    ErrCode Start();

    /*
    * 运行
    * @return ErrCode
    */
    ErrCode Run();

    /*
    * 暂停
    * @return ErrCode
    */
    ErrCode Pause();

    /*
    * 恢复
    * @return ErrCode
    */
    ErrCode Resume();

    /*
    * 停止
    * @return ErrCode
    */
    ErrCode Stop();

    /*
    * 退出
    * @return ErrCode
    */
    ErrCode Exit() noexcept;

    /*
    * 获取状态
    * @return AppState
    */
    AppState GetAppState();

 protected:

 private:
    std::atomic<AppState> m_appState;

    /*
    * 设置服务器状态
    * @return ErrCode
    */
    ErrCode SetAppState(const AppState &);

    bool IsRunning();
};
}// namespace server

#endif// SERVER_APP_H
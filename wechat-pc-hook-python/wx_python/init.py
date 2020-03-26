#coding=utf-8
"""
运行前，安装
python -m pip install --upgrade pip
pip install apscheduler
"""
import os,time
#导入apscheduler线程模块
from apscheduler.schedulers.background import BackgroundScheduler
#导入自定义模块
import tick_read
#运行
if __name__ == '__main__':
    scheduler = BackgroundScheduler()
    # 间隔3秒钟执行一次
    scheduler.add_job(tick_read.tick_read, 'interval', seconds=10, max_instances=1)

    # 这里的调度任务是独立的一个线程
    scheduler.start()
    print('Press Ctrl+{0} to exit'.format('Break' if os.name == 'nt' else 'C'))
    try:
        # 其他任务是独立的线程执行
        while True:
            time.sleep(2)
    except (KeyboardInterrupt, SystemExit):
        scheduler.shutdown()
        print('Exit The Job!')
#ifndef __AVPlayer_H__
#define __AVPlayer_H__
#include <iostream>

typedef void (* pfnPosChanged) (void *data, int iPos);  // �ļ�λ�ñ仯

struct libvlc_instance_t;
struct libvlc_media_player_t;
struct libvlc_event_t;

class CVlcPlay
{
public:
    CVlcPlay(void);
    ~CVlcPlay(void);

    bool Play(const std::string &strPath);  // ����·��ΪstrPath���ļ�
	bool PlaySteam(const std::string &strUrl);
    void Play ();                           // ����
    void Pause();                           // ��ͣ
    void Stop ();                           // ֹͣ
	void Mute();							//����
	BOOL IsMute(); //�Ƿ���
	BOOL SnapShot();	//��ͼ

    void Volume(int nVol);                  // ��������ΪnVol
    void VolumeIncrease();                  // ��������
    void VolumeReduce();                    // ������С 

    void SeekTo(float fPos);                  // ����ָ��λ��nPos
    void SeekForward();                     // ���
    void SeekBackward();                    // ����

    void SetHWND(HWND hwnd);                // ������Ƶ��ʾ�Ĵ��ھ��
    HWND GetHWND();                         // ��ȡ��Ƶ��ʾ�Ĵ��ھ��

    void SetCallback(pfnPosChanged pfn);    // �����ļ�λ�øı�ʱ�Ļص�����
	libvlc_time_t GetLength();
	libvlc_state_t GetState();
    pfnPosChanged GetCallback();     
	void FullSceen();
	void SetTime(libvlc_time_t time);

    BOOL    IsOpen();                       
    BOOL    IsPlaying();                   
    float     GetPos();                       
    __int64 GetTime();                     

protected:
    libvlc_instance_t			*m_pVLC_Inst;  
    libvlc_media_player_t   *m_pVLC_Player; 
	libvlc_time_t					m_TotalTime;
    HWND                    m_hWnd;         // ��Ƶ��ʾ�Ĵ��ھ��
    pfnPosChanged           m_pfn;          // �ļ�λ�øı�ʱ�Ļص�����

private:
    void Init();                        
    void Release();                       
};

#endif

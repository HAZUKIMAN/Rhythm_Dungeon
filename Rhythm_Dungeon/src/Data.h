#pragma once

//　シングルトンの見本でおいているだけなので消します


class Data
{
public:

    //---------------------------------
    // ステージパス一覧
    //---------------------------------
    static constexpr const char* STAGE_PATH[]
    {
        "Data/map/stage1.dat",
        "Data/map/stage2.dat",
        "Data/map/stage3.dat",
        "Data/map/stage4.dat",
        "Data/map/stage5.dat",
        "Data/map/stage6.dat",
        "Data/map/stage7.dat",
        "Data/map/stage8.dat",
        "Data/map/stage9.dat",
    };


    //---------------------------------
    // インスタンス取得
    //---------------------------------
    static Data* GetInstance()
    {
        if (m_instance == nullptr)
        {
            m_instance = new Data;
        }

        return m_instance;
    }

    //---------------------------------
    // インスタンス削除
    //---------------------------------
    static void DeleteInstance()
    {
        if (m_instance != nullptr)
        {
            delete m_instance;
            m_instance = nullptr;
        }
    }

    //---------------------------------
     // ステージ番号保存
     //---------------------------------
    void SetSelectStage(int stage)
    {
        m_selectStage = stage;
    }

    int GetSelectStage() const
    {
        return m_selectStage;
    }

    //---------------------------------
    // 現在のステージパス取得
    //---------------------------------
    const char* GetStagePath() const
    {
        return STAGE_PATH[m_selectStage];
    }


    void NextStage();
    int GetStageCount();

private:

    Data();
    ~Data();

   /* Data(const Data& other);

    Data operator = (const Data& other);*/

    static Data* m_instance;

private:

    int m_selectStage = 0;// 選択ステージ
};

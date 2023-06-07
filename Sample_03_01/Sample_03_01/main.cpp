#include "stdafx.h"
#include "system/system.h"
#include "TrianglePolygon.h"

//関数宣言
void InitRootSignature(RootSignature& rs);

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    //ゲームの初期化
    InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

    //////////////////////////////////////
    // ここから初期化を行うコードを記述する
    //////////////////////////////////////

    //ルートシグネチャを作成
    RootSignature rootSignature;
    InitRootSignature(rootSignature);

    //三角形ポリゴンを定義
    TrianglePolygon triangle;
    triangle.Init(rootSignature);

    // step-1 定数バッファを作成
    ConstantBuffer cb;
    cb.Init(sizeof(Matrix));    //Init関数の引数は定数バッファーのサイズ

    // step-2 ディスクリプタヒープを作成
    DescriptorHeap ds;
    ds.RegistConstantBuffer(0, cb); //ディスクリプタヒープに定数バッファーを登録
    ds.Commit();                    //ディスクリプタヒープへの登録を確定

    //////////////////////////////////////
    // 初期化を行うコードを書くのはここまで！！！
    //////////////////////////////////////
    auto& renderContext = g_graphicsEngine->GetRenderContext();

    float moveX = 0.0f;
    float speedX = 0.125f;
    float moveY = 0.0f;
    float speedY = 0.125f;
    float speedUP = 1.5f;

    bool minus = false;
    // ここからゲームループ
    while (DispatchWindowMessage())
    {
        //フレーム開始
        g_engine->BeginFrame();

        //////////////////////////////////////
        //ここから絵を描くコードを記述する
        //////////////////////////////////////

        //ルートシグネチャを設定
        renderContext.SetRootSignature(rootSignature);

        // step-3 ワールド行列を作成
        Matrix mWorld;
        //mWorld.MakeTranslation(0.5f, 0.4f, 0.0f);
        /*
        moveX = moveX + speedX;
        if (moveX > 2) {
            moveX = -2;
        }
        */

        if (!minus) {
            moveY = moveY + speedY;
        }
        else if (minus) {
            moveY = moveY - speedY * speedUP;   //スピードを変化
        }

        if (moveY > 2) {
            minus = true;
        }
        else if (moveY < -2) {
            minus = false;
        }

        mWorld.MakeTranslation(0.0f, moveY, 0.0f);

        // step-4 ワールド行列をグラフィックメモリにコピー
        cb.CopyToVRAM(mWorld);

        // step-5 ディスクリプタヒープを設定
        renderContext.SetDescriptorHeap(ds);

        //三角形をドロー
        triangle.Draw(renderContext);

        /// //////////////////////////////////////
        //絵を描くコードを書くのはここまで！！！
        //////////////////////////////////////
        //フレーム終了
        g_engine->EndFrame();
    }
    return 0;
}

//ルートシグネチャの初期化
void InitRootSignature( RootSignature& rs )
{
    rs.Init(D3D12_FILTER_MIN_MAG_MIP_LINEAR,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP);
}

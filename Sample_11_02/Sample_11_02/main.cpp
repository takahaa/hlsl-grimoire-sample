#include "stdafx.h"
#include "system/system.h"
#include "ModelStandard.h"

//�֐��錾
void InitRootSignature(RootSignature& rs);

///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐�
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    // �Q�[���̏�����
    InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

    //////////////////////////////////////
    // �������珉�������s���R�[�h���L�q����
    //////////////////////////////////////

    RootSignature rs;
    InitRootSignature(rs);

    // �e�`��p�̃��C�g�J�������쐬����
    Camera lightCamera;

    // �J�����̈ʒu��ݒ�B����̓��C�g�̈ʒu
    lightCamera.SetPosition(0, 500, 0);

    // �J�����̒����_��ݒ�B���ꂪ���C�g���Ƃ炵�Ă���ꏊ
    lightCamera.SetTarget(0, 0, 0);

    // �������ݒ�B����̓��C�g���^���������Ă���̂ŁAX��������ɂ��Ă���
    lightCamera.SetUp(1, 0, 0);

    // ���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���
    lightCamera.Update();

    // �V���h�E�}�b�v�`��p�̃����_�����O�^�[�Q�b�g���쐬����
    float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    RenderTarget shadowMap;
    shadowMap.Create(
        1024,
        1024,
        1,
        1,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_FORMAT_D32_FLOAT,
        clearColor
    );

    // �V���h�E�}�b�v�ɕ`�悷�郂�f��������������
    ModelInitData teapotShadowModelInitData;

    // �V���h�E�}�b�v�`��p�̃V�F�[�_�[���w�肷��
    teapotShadowModelInitData.m_fxFilePath = "Assets/shader/sampleDrawShadowMap.fx";
    teapotShadowModelInitData.m_tkmFilePath = "Assets/modelData/teapot.tkm";
    Model teapotShadowModel;
    teapotShadowModel.Init(teapotShadowModelInitData);
    teapotShadowModel.UpdateWorldMatrix(
        { 0, 50, 0 },
        g_quatIdentity,
        g_vec3One
    );

    // �ʏ�`��̃e�B�[�|�b�g���f����������
    ModelStandard teapotModel;
    teapotModel.Init("Assets/modelData/teapot.tkm");
    teapotModel.Update(
        { 0, 50, 0 },
        g_quatIdentity,
        g_vec3One
    );

    // step-1 �e���󂯂�w�i���f����������

    //////////////////////////////////////
    // ���������s���R�[�h�������̂͂����܂ŁI�I�I
    //////////////////////////////////////
    auto& renderContext = g_graphicsEngine->GetRenderContext();

    // ��������Q�[�����[�v
    while (DispatchWindowMessage())
    {
        // 1�t���[���̊J�n
        g_engine->BeginFrame();

        //////////////////////////////////////
        // ��������G��`���R�[�h���L�q����
        //////////////////////////////////////

        // �V���h�E�}�b�v�Ƀ����_�����O
        // �����_�����O�^�[�Q�b�g���V���h�E�}�b�v�ɕύX����
        renderContext.WaitUntilToPossibleSetRenderTarget(shadowMap);
        renderContext.SetRenderTargetAndViewport(shadowMap);
        renderContext.ClearRenderTargetView(shadowMap);

        // �e���f����`��
        teapotShadowModel.Draw(renderContext, lightCamera);

        // �������݊����҂�
        renderContext.WaitUntilFinishDrawingToRenderTarget(shadowMap);

        // �ʏ탌���_�����O
        // �����_�����O�^�[�Q�b�g���t���[���o�b�t�@�[�ɖ߂�
        renderContext.SetRenderTarget(
            g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
            g_graphicsEngine->GetCurrentFrameBuffuerDSV()
        );
        renderContext.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

        // �e�B�[�|�b�g���f����`��
        teapotModel.Draw(renderContext);

        // step-2 �e���󂯂�w�i��`��

        //////////////////////////////////////
        //�G��`���R�[�h�������̂͂����܂ŁI�I�I
        //////////////////////////////////////
        // 1�t���[���I��
        g_engine->EndFrame();
    }
    return 0;
}

// ���[�g�V�O�l�`���̏�����
void InitRootSignature( RootSignature& rs )
{
    rs.Init(D3D12_FILTER_MIN_MAG_MIP_LINEAR,
            D3D12_TEXTURE_ADDRESS_MODE_WRAP,
            D3D12_TEXTURE_ADDRESS_MODE_WRAP,
            D3D12_TEXTURE_ADDRESS_MODE_WRAP);
}
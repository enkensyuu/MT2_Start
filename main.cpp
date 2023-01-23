#include "DxLib.h"
#include"Vector3.h"

//�֐��錾
//���̕`��
//DxLib = >int DrawSphere3D(VECTOR	ConterPos, float	r, int	DivNum, unsigned	int	DifColor, unsigned	int	Spccolor, unsigned	int	FillFlag);
int DrawSphere3D(const	Vector3& ConterPos, const float	r, const int	DivNum,
	const unsigned	int	DifColor, const unsigned	int	Spccolor, const unsigned	int	FillFlag)
{
	VECTOR	centerPos = { ConterPos.x,ConterPos.y, ConterPos.z };//�\���́@���������X�g

	return	DrawSphere3D(centerPos, r, DivNum, DifColor, Spccolor, FillFlag);//DxLib�֐��Ăяo��
}

//�~���icone�j�̕`��
int	DrawCone3D(const Vector3& TopPos, const Vector3& BottomPos, const float r,
	const	int	DivNum, const	unsigned	int	DifColor, const	unsigned	int	SpcColor, const	int	FillFlag)
{
	VECTOR	topPos = { TopPos.x,TopPos.y, TopPos.z };
	VECTOR	bottomPos{ BottomPos.x,BottomPos.y, BottomPos.z, };

	return	DrawCone3D(topPos, bottomPos, r, DivNum, DifColor, SpcColor, FillFlag);
}

//�����̕`��
//int	DrawLine3D(VECTOR	Pos1, VECTOR	Pos2, unsigned int color);
int	DrawLine3D(const Vector3& Pos1, const Vector3& Pos2, unsigned int color)
{
	VECTOR	p1 = { Pos1.x,Pos1.y,Pos1.z };
	VECTOR	p2 = { Pos2.x,Pos2.y,Pos2.z };

	return	DrawLine3D(p1, p2, color);
}

//�J�����̈ʒu�Ǝp��
int	SetCameraPositionAndTargetAndUpVec(
	const	Vector3& cameraPosition, //�ʒu
	const	Vector3& cameraTarget,	//�����_
	const	Vector3& cameraUp		//�����
)
{
	VECTOR	position = { cameraPosition.x,cameraPosition.y, cameraPosition.z };
	VECTOR	target = { cameraTarget.x,cameraTarget.y, cameraTarget.z };
	VECTOR	up = { cameraUp.x,cameraUp.y, cameraUp.z };

	return	SetCameraPositionAndTargetAndUpVec(position, target, up);
}

void DrawAxis3D(const float length)	//	x,y,z���̕`��
{
	DrawLine3D(Vector3(-length, 0, 0), Vector3(+length, 0, 0), GetColor(255, 0, 0));	//	x��
	DrawLine3D(Vector3(0, -length, 0), Vector3(0, +length, 0), GetColor(0, 255, 0));	//	y��
	DrawLine3D(Vector3(0, 0, -length), Vector3(0, 0, +length), GetColor(0, 0, 255));	//	z��
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	const int WindowWidth = 1024;
	const int WindowHeight = 576;
	ChangeWindowMode(true);//�E�B���h�E���[�h�ɂȂ�
	SetGraphMode(WindowWidth, WindowHeight, 32);//��ʃ��[�h�̃Z�b�g
	SetBackgroundColor(0, 0, 64);//�w�i�FRGB
	if (DxLib_Init() == -1) { return -1; }// DXlib�̏�����
	SetDrawScreen(DX_SCREEN_BACK);// (�_�u���o�b�t�@)�`���O���t�B�b�N�̈�͗��ʂ��w��

	// �y�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(TRUE);

	// �y�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(TRUE);

	//�J����������
	Vector3 cameraPosition(50.0f, 50.0f, -400.0f);
	Vector3 cameraTarget(0.0f, 0.0f, 0.0f);
	Vector3 cameraUp(0.0f, 1.0f, 0.0f);

	//�N���b�v��      �߁@�@�@��
	SetCameraNearFar(1.0f, 1000.0f);//�J�����̗L���͈͂̐ݒ�
	SetCameraScreenCenter(WindowWidth / 2.0f, WindowHeight / 2.0f);//��ʂ̒��S���J�����̒��S�ɍ��킹��
	SetCameraPositionAndTargetAndUpVec(
		Vector3(0.0f, 0.0f, -120.0f),	//	�J�����̈ʒu
		Vector3(0.0f, 0.0f, 0.0f),		//	�J�����̒����_
		Vector3(0.0f, 1.0f, 0.0f)		//	�J�����̏�̌���
	);

	// ���Ԍv���ɕK�v�ȃf�[�^
	long long startCount = 0;
	long long nowCount = 0;
	long long elapsedCount = 0;

	// ��ԂŎg���f�[�^
	// start��end��5[s]�Ŋ���������
	Vector3 start(-100.0f, 0, 0);	//	�X�^�[�g�n�_
	Vector3 end(+100.0f, 0, 0);		//	�G���h�n�_
	float maxTime = 5.0f;			//	�S�̎���[s]
	float timeRate;					//	��%���Ԃ��i�񂾂�(��)

	// �ʂ̈ʒu
	Vector3 position;

	// ���s�O�ɃJ�E���g�l���擾
	startCount = GetNowHiPerformanceCount();	//	long long int�^ 64bit int

	// �Q�[�����[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// �X�V----------------------------

		// [R]�L�[�ŁA���Z�b�g
		if (CheckHitKey(KEY_INPUT_R))
		{
			startCount = GetNowHiPerformanceCount();
		}

		nowCount = GetNowHiPerformanceCount();
		elapsedCount = nowCount - startCount;
		float elapsedTime = static_cast<float>(elapsedCount) / 1'000'000.0f;

		timeRate = min(elapsedTime / maxTime, 1.0f);

		position = lerp(start, end, timeRate);

		// �`��----------------------------
		ClearDrawScreen();
		DrawAxis3D(500.0f);

		// �ʂ̕`��------------------------
		DrawSphere3D(position, 5.0f, 32, GetColor(255, 0, 0), GetColor(255, 255, 255), true);

		//
		DrawFormatString(0, 0, GetColor(255, 255, 255), "position(%5.1f,%5.1f,%5.1f)",
			position.x, position.y, position.z);
		DrawFormatString(0, 20, GetColor(255, 255, 255), "%7.3f[s]", elapsedTime);
		DrawFormatString(0, 40, GetColor(255, 255, 255), "[R]:Restart");

		//�t���b�v����
		ScreenFlip();
	}
	// Dx���C�u�����I������
	DxLib_End();

	// ����I��
	return 0;
}

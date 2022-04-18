#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include<random>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite;
    delete model;}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	texturehandle=TextureManager::Load("mario.jpg");
	sprite=Sprite::Create(texturehandle,{100,50});
	model=Model::Create();
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスター
	std::mt19937_64 engine(seed_gen());
	//乱数範囲
	std::uniform_real_distribution<float> rotDist(0.0, XM_2PI);
	//乱数範囲
	std::uniform_real_distribution<float> posDist(-10.0, 10.0);
	for (size_t i = 0; i < _countof(worldtransform);i++) {
		//スケーリング
		worldtransform[i].scale_ = {2.0f, 2.0f, 2.0f};
		//回転
		worldtransform[i].rotation_ = {rotDist(engine), rotDist(engine), rotDist(engine)};
		//平行移動
		worldtransform[i].translation_ = {posDist(engine), posDist(engine), posDist(engine)};
		//初期化
		worldtransform[i].Initialize();
	}
	//カメラ視点座標を設定
	viewprojection.eye = {0, 0, -10};

	//カメラ注視点座標を設定
	viewprojection.target = {10, 0, 0};

	//カメラ上方向ベクトルを設定(45)
	viewprojection.up = {cosf(XM_PI / 4.0), sinf(XM_PI / 4.0), 0.0f};

	//ビュープロジェクションの初期化
	viewprojection.Initialize();
	//サウンド
	/*sounddatahandle=audio_->LoadWave("se_sad03.wav");
	audio_->PlayWave(sounddatahandle);
	voicehandle=audio_->PlayWave(sounddatahandle,true);*/
	
}

void GameScene::Update() {
//XMFLOAT2 position=sprite->GetPosition();
//position.x+=2.0;
//position.y+=1.0;
//sprite->SetPosition(position);

if (input_->TriggerKey(DIK_SPACE)) {
	//
	audio_->StopWave(voicehandle);
}
//変数の値をインクリメント
//value++;
//値を含んだ文字列/////////////////
/*std::string strdebug=std::string("value:")+
std::to_string(value);
//デバッグテキストの表示
debugText_->Print(strdebug,50,50,1.0);*/

//視点移動処理
{
	//視点の移動ベクトル
	XMFLOAT3 move = {0, 0, 0};

	//視点の移動速度
	const float kEyeSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_W)) {
		move = {0, 0, kEyeSpeed};
	} else if (input_->PushKey(DIK_S)) {
		move = {0, 0, -kEyeSpeed};
	}

	//視点移動（ベクトルの加算）
	viewprojection.eye.x += move.x;
	viewprojection.eye.y += move.y;
	viewprojection.eye.z += move.z;

	//行列の再計算
	viewprojection.UpdateMatrix();

	//デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "eye:(%f,%f,%f)", viewprojection.eye.x, viewprojection.eye.y, viewprojection.eye.z);
}

//注視点移動処理
{
	//注視点の移動ベクトル
	XMFLOAT3 move = {0, 0, 0};

	//視点の移動速度
	const float kTargetSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move = { -kTargetSpeed,0,0};
	} else if (input_->PushKey(DIK_RIGHT)) {
		move = {kTargetSpeed,0,0};
	}

	//注視点移動（ベクトルの加算）
	viewprojection.target.x += move.x;
	viewprojection.target.y += move.y;
	viewprojection.target.z += move.z;

	//行列の再計算
	viewprojection.UpdateMatrix();

	//デバッグ用表示
	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "target:(%f,%f,%f)", viewprojection.target.x, viewprojection.target.y, viewprojection.target.z);
}
//上方向回転処理
{
	//
	const float kUpRotSpeed = 0.05;

	//
	if (input_->PushKey(DIK_SPACE)) {
		viewAngle += kUpRotSpeed;
		//
		viewAngle = fmodf(viewAngle, XM_2PI);
	}

	//
	viewprojection.up = {cosf(viewAngle), sinf(viewAngle), 0.0};

	//
	viewprojection.UpdateMatrix();

	//デバッグ
	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "up:(%f,%f,%f)", viewprojection.up.x, viewprojection.up.y,
	  viewprojection.up.z);
}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	//sprite->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	for (size_t i = 0; i < _countof(worldtransform); i++) {
		model->Draw(worldtransform[i], viewprojection, texturehandle);
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

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
	worldtransform.scale_={5.0f,5.0f,5.0f};
	worldtransform.rotation_={2.3,XM_PI/5.5,3.0};
	worldtransform.translation_ = {10.0, 10.0, 10.0};
	worldtransform.Initialize();
	viewprojection.Initialize();
	/*sounddatahandle=audio_->LoadWave("se_sad03.wav");
	audio_->PlayWave(sounddatahandle);
	voicehandle=audio_->PlayWave(sounddatahandle,true);*/
	
}

void GameScene::Update() {/////
//XMFLOAT2 position=sprite->GetPosition();
//position.x+=2.0;
//position.y+=1.0;
//sprite->SetPosition(position);

if (input_->TriggerKey(DIK_SPACE)) {
	//
	audio_->StopWave(voicehandle);
}
//変数の値をインクリメント
value++;
//値を含んだ文字列/////////////////
/*std::string strdebug=std::string("value:")+
std::to_string(value);
//デバッグテキストの表示
debugText_->Print(strdebug,50,50,1.0);*/
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
	model->Draw(worldtransform,viewprojection,texturehandle);

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

#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	uint32_t texturehandle=0;
	Sprite* sprite=nullptr;
	Model* model=nullptr;
	uint32_t sounddatahandle=0;
	uint32_t voicehandle=0;
	uint32_t value=0;
	//カメラ上方向の角度
	float viewAngle = 0.0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	//ワールドトランスフォーム
	WorldTransform worldtransform[100];
	ViewProjection viewprojection;

	
};

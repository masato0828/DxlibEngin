現在制作中のゲームエンジンです
仕様ライブラリはDXライブラリ、ImGuiです

現在の機能としては、
・モデルの読み込み(MV1形式)
(MV1への変換ツールはDxlib公式のDxlibModelViewerがいります。)
・シェーダーの動的コンパイル
・ファイルダイアログ
です。

〇操作について
・sceneview内でマウス右を押下中マウスを動かすとカメラの視点移動


〇シェーダーの動的コンパイルについて
1.Projectタブ➔dataファイル➔modelDataファイル➔好きなモデルを選択
2.Itemsタブ➔モデルをクリック(選択した文字が青くなります)➔右側shaderLoadボタンを押す➔読み込みたいHLSLファイルを選択
(動作ファイル/Shader/Pixel/)
・ReflectionPixelShader.hlsl	:鏡面反射
・PearlShader.hlsl		:パールシェーダー
・ModelPixelShader　　　　　　　:通常モデル
・ModelNormalPixelShader	:ノーマルマップありのモデル
3.コンパイル完了

4.直接ファイルの数値を変える
例：	ReflectionPixelShader.hlsl➔13行目
	float4 cubeColor = 1.0f * cubeMapTexture.Sample(cubeMapSampler,cubeRef);
	1.0fになっている所を0.0fなどに変える

5.shaderReloadボタンを押す
	変更完了

〇オプションについて
ウィンドウ上部のsystemをクリック➔optionをクリック➔各タブにてパラメータの変更やポストエフェクトの確認ができます。

〇モデルについて
1.Projectタブ➔dataファイル➔modelDataファイル➔好きなモデルを選択
2.モデルをドラッグアンドドロップでsceneViewに投げるとモデルが表示されます

実装できていない箇所について
・CodeEditerウィンドウにてコードの表示、変更はできるのですが、
	コードの変更をした際の保存ができていない状態です。
・モデルのTextureの変更
・データの読込及び出力
・その他etc…

注意事項
・ウィンドウ右上部の　[-]を押すと画面が消えます（仕様です）
　その際画面左上に黒い小さい四角が出ます
　その状態でOを押すと元に戻ります



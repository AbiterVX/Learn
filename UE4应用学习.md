### 0.Tips

（1）无法在构造函数中NewObject

（2）无法在蓝图构造函数中SpawnActor

（3）UStruct不能为指针

​	UE4中蓝图的展示，只适用于继承于UObject的指针

（4）网络同步中，Pawn通过Movement移动（AddInputVector）时才会同步范围内的actor。

使用SetActorLocation并不会同步范围内Actor，并且会自动销毁范围外的actor。

（5）网络同步的Map与Set无法不支持同步Replicated。

（6）注意在需要同步时，对Event和Properties勾选Replicated。



### 1.Git与GitHub

​	配置参考：<https://www.bilibili.com/video/BV15f4y1U7ui>

​	.gitignore文件：<https://github.com/github/gitignore/blob/master/UnrealEngine.gitignore>

​	.gitignore文件放在UE4项目目录下。

### 2.xlsx转json工具

​	github：<https://github.com/koalaylj/xlsx2json>

​	需安装nodejs环境

http://nodejs.cn/download/

配置环境变量：解压的路径放入系统变量的Path中

运行指令：npm install

​	配置config.json文件。

### 3.设置人物

Character/self

Pawn->AutoPossessPlayer 设置值。

UseControllerRotation 为 Controller跟随旋转。

### 4.相机跟随鼠标旋转

SpringArm ： CameraSettings 设置UsePawnControlRotation，

​		       并且Inherit Pitch/Yaw/Roll 勾选才可以使得相机跟随旋转。

Camera：UsePawnControlRotation不设置。

### 5.Input设置

```C++
//绑定输入Axis 
PlayerInputComponent->BindAxis("MoveForward", this, &ASurvivalCharacter::MoveForward);
//鼠标旋转可直接使用Pawn里的函数。或者包装，并使用AddControllerYawInput(Rate)函数
PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInpu);
```

### 6.光照

WorldSetting

​	Lightmass/ForceNoPrecomputedLighting 设为True

DirectonalLight组件: 主光源

​			Mobility设为Movable

​			Light/CastShadows设为True

BP_Sky_Sphere组件：天空盒

​	Default/DirectionalLightActor 设为对应光源

​	每次旋转调整光源角度需要点击 Default/RefreshMaterial 进行刷新。	



### 7.添加模块

参考链接：

<https://blog.csdn.net/qq_36409711/article/details/100783445?utm_medium=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase>

<https://blog.csdn.net/u011349044/article/details/80107395>

步骤：

1.在source目录下建立文件夹，并命名为模块名

2.在文件夹下创建同名的 .h  .cpp  .Build.cs 三个文件。（可参考已有的文件并修改对应名称）

3.在项目的两个  .Target.cs 文件内添加代码

```CS
	ExtraModuleNames.Add("Nature");
```

4.在项目的 .uproject 文件内的Modules里添加

```json
	{
		"Name": "模块名",
		"Type": "Runtime",
		"LoadingPhase": "Default",
		"AdditionalDependencies": [
			"Engine"
		]
	}
```



### 8.VS导入文件

VS中添加现有项相当于一个指针，它指向所添加的文件，但不会主动将添加的文件放到当前项目中。若未将.h文件复制项目中，而直接添加现有项，虽然解决方案中含有.h，但实际的.h依旧在项目外，程序无法编译运行。



### 9.删除C++ 类

1.关闭UE4，在VS中删除类文件。

2.在工程目录中(Source)删除对应文件。

3.删除Binaries文件夹。

4.在VS里生产解决方案

5.打开UE4

### 10.C++ Json库

链接：<https://github.com/nlohmann/json>

配置：直接导入single_include\nlohmann 下的json.hpp文件



### 11.类型转换

<https://zhuanlan.zhihu.com/p/37726745>

### 12.时间

1.获取时间

<https://docs.unrealengine.com/en-US/API/Runtime/Engine/Engine/UWorld/GetTimeSeconds/index.html>

```c++
	UWorld* world;
	float timeSeconds = world->GetTimeSeconds();
```

2.计时器

```c++
	//定义句柄，从 0.0f开始 每 1.0f执行一次函数。
	FTimerHandle MemberTimerHandle;
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AWorldTimer::TestFunction, 1.0f, true, 0.0f);
	
	//打印
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::SanitizeFloat(timeSeconds));
```

### 13.地形

地形碰撞：使用物体表面作为碰撞

Collision/CollisionComplexity: use Complex Collision as Simple



绘画材质：

<https://www.bilibili.com/video/av77796366/>



 密度：Painting/Density 

垂直放置：Placement/AligntoNormal 

视角过远不显示：InstanceSettings/CullDistance

### 14.UI

（1）蓝图显示UI

​	CreateWidget  之后 AddToViewPort



（2）C++ 显示UI

```c++
//定义
UFUNCTION(BlueprintCallable, Category = "UI")
void showUIWidget(UClass* currentClass);

//实现
void UMainGameInstance::showUIWidget(UClass* currentClass)
{
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	UUserWidget* widgetInst = CreateWidget<UUserWidget>(playerController, currentClass);
	if (widgetInst != nullptr) {
		widgetInst->AddToViewport();
	}
}
```



### 15.海洋Material

<https://www.worldofleveldesign.com/categories/ue4/materials-starter-content-water-instance.php>

### 16.蓝图添加参数

按住s+鼠标左键。  或者右键搜索 constant 并转为paramter。   搜索scale parmater

### 17.判断浮点数相等

蓝图：NearlyEqual

C++：FMath::IsNearlyEqual()

### 18.动画

添加状态机：右键 AddNewStateMachine

### 19.动画重定向

#### 1关节预处理

调整当前模型的骨骼（UE4_Mannequin_Skeletomesh）与所要重定向的骨骼保持一致。

打开骨骼文件并选择RetargetManger

将Select Rig 设为 Humanoid

调整姿势

将ModifyPose 设为 UseCurrentPose

保存

#### 2重定向的骨骼

打开索要重定向的骨骼文件

选择Select Rig 设为 Humanoid

之后对应每个关节，包括base和advance的关节。

保存

#### 3重定向动画

选择转换的Animation ，点击RetargetManger。



可能需要先将骨骼重定向：点击骨骼文件右键 RetargetToAnotherSkelton

#### 4设置骨骼最低点位置

SkeletonTree/options/ShowRetargetingOptions

设置TranslationRetargeting



root设为Animation

Hips设为AnimationScaled

其他设为Skeleton

(2)导入ConstructorHelpers

```c++
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
```

### 20.资源动态加载



https://blog.csdn.net/a359877454/article/details/52765133

### 21.Debug

```c++
#include "Engine.h"

GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, testString);
```



### 22.Image九宫格

Image控件的Appearance/DrawAs设为Box

使用Margin调整大小



### 23.创建C++对象，SPawn

https://blog.csdn.net/qq_20309931/article/details/52872965



### 24.蓝图内调用父类事件

事件上，右键add call to parent function



### 25.蓝图设置对齐

根据父控件类型 cast to HorizontalBoxSlot/VerticalBoxSlot   （其一）

之后 set HorizontalAlignment/VerticalAlignment       水平/垂直的对其方式



### 26.人物骨骼模型创建Socket

[https://blog.csdn.net/wag2765/article/details/84763460%EF%BC%9F](https://blog.csdn.net/wag2765/article/details/84763460)





### 27.Input输入，带参数绑定

https://answers.unrealengine.com/questions/144613/bindaction-delegate-with-parameters.html



### 27.查找Actor

https://blog.csdn.net/qq_35014708/article/details/86543205



### 28.相机碰撞问题

SpringArm的CameeraCollision/DpCollisionTest 设为false



### 29.C++ 相机

引入相机（PS：编译后，BP里无法显示detail，存在bug）

```C++
//.h
UPROPERTY(EditAnywhere,BlueprintReadWrite,Category ="Camera")
class USpringArmComponent* springArmComp;
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
class UCameraComponent* cameraComp;

//.CPP
springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
springArmComp->bDoCollisionTest = false;
springArmComp->bUsePawnControlRotation = true;
cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
springArmComp->SetupAttachment(RootComponent);
cameraComp->SetupAttachment(springArmComp);
```



get到相机

```c++
APlayerCameraManager* currentCamera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
//位置
FVector CameraLocation = currentCamera->GetCameraLocation();
//方向
FVector direction = currentCamera->GetActorForwardVector();
```



### 30.画线

```c++
DrawDebugLine(GetWorld(), posBegin, posEnd, FColor::Red, false, 0.0f,0.0f,10.0f);
```





### 31.C++枚举类

https://www.pianshen.com/article/9321941007/

### 32.事件调度

https://blog.ch-wind.com/ue4-event-dispatcher-and-delegate/

https://www.mengmei.moe/note/unrealengine/273.html

### 33.鼠标拖拽



### 34.VisualAssistX

https://blog.csdn.net/weixin_30477797/article/details/102086246

https://blog.csdn.net/RyanDong233/article/details/79169307



### 35.Timer

```c++
//GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AWorldTimer::timePassPerSecond, 1.0f, true, 0.0f);

World->GetTimerManager().SetTimer(TimerHandle,Callback,Rate,bLoop,FirstDelay);
```

### 36.材质

https://www.bilibili.com/video/BV1GJ411j7d4?p=17 



Distance to nearest surface只在 engine scalability的Epic画质以上显示。

且需开启generate distance field



### 37.画质

获取画质

https://answers.unrealengine.com/questions/212429/how-to-get-current-scalability-setting.html

设置画质

https://answers.unrealengine.com/questions/540120/can-you-set-the-engine-scalability-values-lowmediu.html







### 38.结构

Actor：跨关卡

Component组件：能力是跨游戏的，与业务逻辑无关。

Pawn:可被控制的。写肉体的功能

AController:控制Acotr的Actor

APlayerState：肉体状态，为了联机复制

PLayerController: 输入，UI，。。。

GameInstance：UI

ULevel：

UWorld：Level集合，



AGameMode: 规则，服务器逻辑。

AGameState:  

UGameInstance:整场游戏，游戏exe打开到退出。World外围管理对象

USaveGame:游戏存档  经验值 



网络同步是Actor



USbusytem:业务逻辑组件，5个生命周期粒度控制。自动生成对象，托管生命周期，替代Manager类。

![image-20200727210357052](C:\Users\57641\AppData\Roaming\Typora\typora-user-images\image-20200727210357052.png)



![image-20200727210447685](C:\Users\57641\AppData\Roaming\Typora\typora-user-images\image-20200727210447685.png)



![image-20200727210856361](C:\Users\57641\AppData\Roaming\Typora\typora-user-images\image-20200727210856361.png)



![image-20200727211003750](C:\Users\57641\AppData\Roaming\Typora\typora-user-images\image-20200727211003750.png)









### 39.数据表

c++使用数据表

https://zhuanlan.zhihu.com/p/80324324



### 40.TArray 

sort reference https://answers.unrealengine.com/questions/606320/help-with-c-sort-by-predicate.html

### 41.TMap

https://blog.csdn.net/u011476173/article/details/98769865





### 42.圆环进度条

https://www.bilibili.com/video/av77958549/



### 43.富文本

可在文字里添加图片。

https://www.youtube.com/watch?v=msy7bRDpKF4

### 44.初始化Const static 变量

https://answers.unrealengine.com/questions/494023/compiler-error-when-creating-static-const-fstring.html



### 45网络

#### Replication

​	信息从服务端同步到客户端（单向）

​	Actor及其派生类才有Replicaion能力

#### 类型

##### 	Actor Replication

​		从服务端生成，若开启replication，客户端也会生成（在服务端生成一个Replicatie 对象）

​		若开启，才能进行：当前Actor的所有属性复制、组件复制、RPC的总开关



​		开启方式：

​			蓝图：勾选ClassDefaults ->Replication - > Replicates

​			C++： 构造函数中 bReplicates = true；

##### 	Property Replication

​		Variable类型

​		开启方式：

​			蓝图：勾选Details - > Replication - > Replicates

​			C++： 1.属性前增加UPROPERTY（Replicated）

​						2.  .cpp文件中，在 GetLifetimeReplicatedProps函数中添加

​							DOREPLIFETIME（类名称，变量名）不需要声明该函数

```c++
#include "Net/UnrealNetwork.h" 
void ClassName::GetLifetimeReplicatedProps(TArray<FLiefetimeProperty> & OutLifetimeProps) const{
 	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ClassName, var_1);
 }

if(HasAuthrity()){
    
}
```



##### 	Component Replication



#### 复制

​	Switch has Authority

​		服务端：Authority

​		客户端：Remote

​	在服务端生成在客户端Replicate。这样防止在每个玩家处生成。

#### 测试

​	Play： 

​		人数设为2+， 

​		模式：Play as listener server

​		New Editor Window



#### RepNotify

​	设置：变量的Replication -> RepNotify

​	如果一个变量设置为Rep_Notify，当该变量发生复制时，服务端和收到该值的客户端都可以调用一个自定义的函数（复制通知：变量每次发生复制就执行一个函数）

​	C++的版本略有区别，仅在客户端调用函数。	

​	设置

​		蓝图：设置为RepNotify即自动生成

​		C++：

```c++
	UPROPERTY（ReplicatedUsing = OnRep_Var_1）
	float var_1
    
    UFUNCTION()
    void OnRep_Var_1(){}
```





### 46.Splines样条线

创建：蓝图中创建一个Actor然后添加一个Component（Spline Mesh）

Spline Generation



sequencer



### 47.InstanaceStaticMesh



### 48.创建蓝图对象

### 49.重新构建VS项目

文件删除： Binaries

文件备份： .vs    Intermediate   Saved    .sln

.uproject 右键 ：生成vs文件

打开.sln 文件然后build

### 49.类内static成员与类内static函数

https://blog.csdn.net/FreeApe/article/details/50979425

### 50.设置PlayerStart位置

比用户登录Event更前执行。

https://forums.unrealengine.com/development-discussion/blueprint-visual-scripting/121627-basic-guide-how-to-select-a-player-start-in-multiplayer

### 51.网络复制

UObject复制

https://www.cnblogs.com/leonhard-/p/6511821.html

https://www.programmersought.com/article/55561427808/

### 52.与Github关联

#### 52.1本地项目创建

（1）将Gitignore文件放到项目中

https://github.com/github/gitignore/blob/master/UnrealEngine.gitignore

（2）如本地工程使用git需提前进行准备工作

​	先生成ssh Key

```
ssh-keygen -t rsa -C "576412173@qq.com"
```

​	获取本地生成的key（复制文件内容），文件路径：C:\Users\LIIHZ\.ssh\id_rsa.pub。

​	到github头像->setting->key设置处添加key

​	之后设置配置：

```
git config --global user.name "abitervx"
git config --global user.email "576412173@qq.com"
```

​	再添加远程仓库

```
git remote add origin git@github.com:AbiterVX/DiceTown.git
```

（2）git项目初始化

```
//初始化
git init
//添加文件
git add .
//查看
git status

//[可忽略]创建main分支
git branch -M main
//添加远端地址
git remote add origin git@github.com:AbiterVX/DiceTown.git

//提交提交
git commit -m "init commit 初始化"

// push到github main分支
git push -u origin main

//查看历史提交
git log

//查看远程分支
git branch -a

//查看本地分支
git branch

//创建分支
git branch test
```

### 52.3后续其他操作

将远程仓库数据同步到本地

```
git pull origin master
```

### 52.4大文件上传

使用git lfs

https://blog.csdn.net/u012390519/article/details/79441706

```
//在当前git环境安装lfs
git lfs install
//追踪指定类型的文件
git lfs track "*.umap"
```

### 52.5拉取远程项目

拉取项目

```
git clone git@github.com:AbiterVX/DiceTown.git
```


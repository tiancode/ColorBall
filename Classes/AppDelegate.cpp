#include "AppDelegate.h"
#include "SplashScene.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "C2DXShareSDK.h"
using namespace cn::sharesdk;
#endif



USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//初始化ShareSDK
	C2DXShareSDK::open("api20", false);

	//初始化社交平台信息
	//新浪微博
	__Dictionary *sinaConfigDict = __Dictionary::create();
	sinaConfigDict->setObject(String::create("324324324243"), "app_key");
	sinaConfigDict->setObject(String::create("dsjfe759fdk4972jfj83ir434739"), "app_secret");
	sinaConfigDict->setObject(String::create("http://weibo.com/2415895697"), "redirect_uri");
	C2DXShareSDK::setPlatformConfig(C2DXPlatTypeSinaWeibo, sinaConfigDict);

	//Twitter
	__Dictionary *twConfigDict = __Dictionary::create();
	twConfigDict->setObject(String::create("UjfueJYrudjYJUfkeiJFKf"), "consumer_key");
	twConfigDict->setObject(String::create("FJRUkddaljfklsfiejkFHUREY*T$*HIieurlkdjlfsfs"), "consumer_secret");
	twConfigDict->setObject(String::create("https://twitter.com/tian_sh"), "redirect_uri");
	C2DXShareSDK::setPlatformConfig(C2DXPlatTypeTwitter, twConfigDict);

#endif


    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("ColorBall");
        director->setOpenGLView(glview);
    }

	//glview->setFrameSize(480, 720);
	//glview->setDesignResolutionSize(480, 720,ResolutionPolicy::SHOW_ALL);

    // turn on display FPS
    //director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    register_all_packages();

    // create a scene. it's an autorelease object
    auto scene = SplashScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}


#include "cons.h"

using namespace cocos2d;
using namespace cocostudio;


IMPLEMENT_CLASS_INFO(TimeElapsed)

TimeElapsed::TimeElapsed(void)
:_totalTime(0.0f)
,_tmpTime(0.0f)
,_scheduler(nullptr)
,_suc(false)
{
	_scheduler = Director::getInstance()->getScheduler();
	_scheduler->retain();
}

TimeElapsed::~TimeElapsed(void)
{
	CC_SAFE_RELEASE(_scheduler);
}

bool TimeElapsed::init()
{
	_scheduler->schedule(schedule_selector(TimeElapsed::update), this, 0.0f , kRepeatForever, 0.0f, false);
    return true;
}

bool TimeElapsed::detect()
{
    return _suc;
}

void TimeElapsed::serialize(const rapidjson::Value &val)
{
    int count = DICTOOL->getArrayCount_json(val, "dataitems");
	for (int i = 0; i < count; ++i)
	{
		const rapidjson::Value &subDict = DICTOOL->getSubDictionary_json(val, "dataitems", i);
		std::string key = DICTOOL->getStringValue_json(subDict, "key");
		if (key == "TotalTime")
		{
			_totalTime = DICTOOL->getFloatValue_json(subDict, "value");
		}
	}
}

void TimeElapsed::serialize(cocostudio::CocoLoader *pCocoLoader, cocostudio::stExpCocoNode *pCocoNode)
{
	int length = pCocoNode->GetChildNum();
	int count = 0;
	stExpCocoNode *pDataItemsArray = pCocoNode->GetChildArray(pCocoLoader);
	std::string key;
	const char *str = nullptr;
	for (int i = 0; i < length; ++i)
	{
		count = pDataItemsArray[i].GetChildNum();
		stExpCocoNode *pDataItemArray = pDataItemsArray[i].GetChildArray(pCocoLoader);
		key = pDataItemArray[0].GetValue(pCocoLoader);
		str = pDataItemArray[1].GetValue(pCocoLoader);
		if (key == "TotalTime")
		{
			if (str != nullptr)
			{
				_totalTime = atof(str); //DICTOOL->getFloatValue_json(subDict, "value");
			}
		}
	}
}
void TimeElapsed::removeAll()
{
	_scheduler->unschedule(schedule_selector(TimeElapsed::update), this);
}

void TimeElapsed::update(float dt)
{
	_tmpTime += dt;
	if (_tmpTime > _totalTime)
	{
		_tmpTime = 0.0f;
		_suc = true;
	}
}

IMPLEMENT_CLASS_INFO(ArmatureActionState)
ArmatureActionState::ArmatureActionState(void)
: _tag(-1)
, _state(-1)
, _suc(false)
{
}

ArmatureActionState::~ArmatureActionState(void)
{
}

bool ArmatureActionState::init()
{
	do 
	{
		Node *pNode = SceneReader::getInstance()->getNodeByTag(_tag);
		CC_BREAK_IF(pNode == nullptr);
		ComRender *pRender = (ComRender*)(pNode->getComponent(_comName.c_str()));
		CC_BREAK_IF(pRender == nullptr);
		Armature *pAr = (Armature *)(pRender->getNode());
		CC_BREAK_IF(pAr == nullptr);
		TriggerMng::getInstance()->addArmatureMovementCallBack(pAr, this, movementEvent_selector(ArmatureActionState::animationEvent));
	} while (0);
	
	return true;
}

bool ArmatureActionState::detect()
{
    return _suc;
}

void ArmatureActionState::serialize(const rapidjson::Value &val)
{
	int count = DICTOOL->getArrayCount_json(val, "dataitems");
	for (int i = 0; i < count; ++i)
	{
		const rapidjson::Value &subDict = DICTOOL->getSubDictionary_json(val, "dataitems", i);
		std::string key = DICTOOL->getStringValue_json(subDict, "key");
		if (key == "Tag")
		{
			_tag = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
		else if (key == "componentName")
		{
			_comName = DICTOOL->getStringValue_json(subDict, "value");
			continue;
		}
		else if (key == "AnimationName")
		{
			_aniname = DICTOOL->getStringValue_json(subDict, "value");
			continue;
		}
		else if (key == "ActionType")
		{
			_state = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
	}
}

void ArmatureActionState::serialize(cocostudio::CocoLoader *pCocoLoader, cocostudio::stExpCocoNode *pCocoNode)
{
	int length = pCocoNode->GetChildNum();
	int count = 0;
	stExpCocoNode *pDataItemsArray = pCocoNode->GetChildArray(pCocoLoader);
	std::string key;
	const char *str = nullptr;
	for (int i = 0; i < length; ++i)
	{
		count = pDataItemsArray[i].GetChildNum();
		stExpCocoNode *pDataItemArray = pDataItemsArray[i].GetChildArray(pCocoLoader);
		key = pDataItemArray[0].GetValue(pCocoLoader);
		str = pDataItemArray[1].GetValue(pCocoLoader);
		if (key == "Tag")
		{
			if (str != nullptr)
			{
				_tag = atoi(str);//DICTOOL->getIntValue_json(subDict, "value");
			}
		}
		else if (key == "componentName")
		{
			if (str != nullptr)
			{
				_comName = str; //DICTOOL->getStringValue_json(subDict, "value");
			}
			
		}
		else if (key == "AnimationName")
		{
			if (str != nullptr)
			{
				_aniname = str; //DICTOOL->getStringValue_json(subDict, "value");
			}
		}
		else if (key == "ActionType")
		{
			if (str != nullptr)
			{
				_state = atoi(str); //DICTOOL->getIntValue_json(subDict, "value");
			}
		}
	}
}
void ArmatureActionState::removeAll()
{
	do 
	{
		Node *pNode = SceneReader::getInstance()->getNodeByTag(_tag);
		CC_BREAK_IF(pNode == nullptr);
		ComRender *pRender = (ComRender*)(pNode->getComponent(_comName.c_str()));
		CC_BREAK_IF(pRender == nullptr);
		Armature *pAr = (Armature *)(pRender->getNode());
		CC_BREAK_IF(pAr == nullptr);
		TriggerMng::getInstance()->removeArmatureMovementCallBack(pAr, this, movementEvent_selector(ArmatureActionState::animationEvent));
	} while (0);
}

void ArmatureActionState::animationEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID)
{
	 std::string id = movementID;
	if (movementType == _state && id.compare(_aniname) == 0)
	{
		_suc = true;
	}
}

IMPLEMENT_CLASS_INFO(NodeInRect)
NodeInRect::NodeInRect(void)
:_tag(-1)
{
}

NodeInRect::~NodeInRect(void)
{
}

bool NodeInRect::init()
{

    return true;
}

bool NodeInRect::detect()
{
	Node *pNode = SceneReader::getInstance()->getNodeByTag(_tag);
	if (pNode != nullptr && abs(pNode->getPositionX() - _origin.x) <= _size.width && abs(pNode->getPositionY() - _origin.y) <= _size.height)
	{
		return true;
	}
    return false;
}

void NodeInRect::serialize(const rapidjson::Value &val)
{
	int count = DICTOOL->getArrayCount_json(val, "dataitems");
	for (int i = 0; i < count; ++i)
	{
		const rapidjson::Value &subDict = DICTOOL->getSubDictionary_json(val, "dataitems", i);
		std::string key = DICTOOL->getStringValue_json(subDict, "key");
		if (key == "Tag")
		{
			_tag = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
		else if (key == "originX")
		{
			_origin.x = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
		else if (key == "originY")
		{
			_origin.y = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
		else if (key == "sizeWidth")
		{
			_size.width = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
		else if (key == "sizeHeight")
		{
			_size.height = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
	}
}

void NodeInRect::serialize(cocostudio::CocoLoader *pCocoLoader, cocostudio::stExpCocoNode *pCocoNode)
{
	int length = pCocoNode->GetChildNum();
	int count = 0;
	stExpCocoNode *pDataItemsArray = pCocoNode->GetChildArray(pCocoLoader);
	std::string key;
	const char *str = nullptr;
	for (int i = 0; i < length; ++i)
	{
		count = pDataItemsArray[i].GetChildNum();
		stExpCocoNode *pDataItemArray = pDataItemsArray[i].GetChildArray(pCocoLoader);
		key = pDataItemArray[0].GetValue(pCocoLoader);
		str = pDataItemArray[1].GetValue(pCocoLoader);
		if (key == "Tag")
		{
			if (str != nullptr)
			{
				_tag = atoi(str);//DICTOOL->getIntValue_json(subDict, "value");
			}
		}
		else if (key == "originX")
		{
			if (str != nullptr)
			{
				_origin.x = atoi(str); //DICTOOL->getIntValue_json(subDict, "value");
			}
		}
		else if (key == "originY")
		{
			if (str != nullptr)
			{
				_origin.y = atoi(str); //DICTOOL->getIntValue_json(subDict, "value");
			}
		}
		else if (key == "sizeWidth")
		{
			if (str != nullptr)
			{
				_size.width = atoi(str); //DICTOOL->getIntValue_json(subDict, "value");
			}
		}
		else if (key == "sizeHeight")
		{
			if (str != nullptr)
			{
				_size.height = atoi(str); //DICTOOL->getIntValue_json(subDict, "value");
			}
		}
	}
}
void NodeInRect::removeAll()
{
	CCLOG("NodeInRect::removeAll");
}


IMPLEMENT_CLASS_INFO(NodeVisible)
NodeVisible::NodeVisible(void)
: _tag(-1)
, _visible(false)
{
}

NodeVisible::~NodeVisible(void)
{
}

bool NodeVisible::init()
{
	return true;
}

bool NodeVisible::detect()
{
	Node *pNode = SceneReader::getInstance()->getNodeByTag(_tag);
	if (pNode != nullptr && pNode->isVisible() == _visible)
	{
		return true;
	}
	return false;
}

void NodeVisible::serialize(const rapidjson::Value &val)
{
	int count = DICTOOL->getArrayCount_json(val, "dataitems");
	for (int i = 0; i < count; ++i)
	{
		const rapidjson::Value &subDict = DICTOOL->getSubDictionary_json(val, "dataitems", i);
		std::string key = DICTOOL->getStringValue_json(subDict, "key");
		if (key == "Tag")
		{
			_tag = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
		else if (key == "Visible")
		{
			_visible = DICTOOL->getIntValue_json(subDict, "value") ? true : false;
			continue;
		}
	}
}

void NodeVisible::serialize(cocostudio::CocoLoader *pCocoLoader, cocostudio::stExpCocoNode *pCocoNode)
{
	int length = pCocoNode->GetChildNum();
	int count = 0;
	stExpCocoNode *pDataItemsArray = pCocoNode->GetChildArray(pCocoLoader);
	std::string key;
	const char *str = nullptr;
	for (int i = 0; i < length; ++i)
	{
		count = pDataItemsArray[i].GetChildNum();
		stExpCocoNode *pDataItemArray = pDataItemsArray[i].GetChildArray(pCocoLoader);
		key = pDataItemArray[0].GetValue(pCocoLoader);
		str = pDataItemArray[1].GetValue(pCocoLoader);
		if (key == "Tag")
		{
			if (str != nullptr)
			{
				_tag = atoi(str);//DICTOOL->getIntValue_json(subDict, "value");
			}
		}
		else if (key == "Visible")
		{
			if (str != nullptr)
			{
				_visible = atoi(str) != 0? true:false;//DICTOOL->getIntValue_json(subDict, "value") != 0? true:false;
			}
		}
	}
}
void NodeVisible::removeAll()
{
	CCLOG("NodeVisible::removeAll");
}





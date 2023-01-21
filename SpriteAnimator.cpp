#define _CRT_SECURE_NO_WARNINGS
#include "sajson.h"

#include "SpriteAnimator.h"
#include <imgui.h>

SpriteAnimator::SpriteAnimator(Sprite* sprite)
	: ref(sprite)
	, currentAnimation(nullptr)
	, entryAnimation(nullptr)
	, animationChange(false)
	, animationRefFileExists(false)
	, animationRefFile("NA")
{
}

SpriteAnimator::SpriteAnimator(Sprite* sprite, const char* filepath)
	: ref(sprite)
	, currentAnimation(nullptr)
	, entryAnimation(nullptr)
	, animationChange(false)
	, animationRefFileExists(filepath != nullptr)
	, animationRefFile(filepath)
{
	LoadAnimationsFromFile_json(filepath);
}

SpriteAnimator::~SpriteAnimator()
{
	ref = nullptr;
	clearAnimations();
}

void SpriteAnimator::clearAnimations()
{
	animationMap.clear();
	currentAnimation = nullptr;
	entryAnimation = nullptr;

	auto iter = Animations.begin();
	while (iter != Animations.end())
	{
		Animation* anim = (*iter);
		iter = Animations.erase(std::remove(Animations.begin(), Animations.end(), anim), Animations.end());
		delete anim;
		anim = nullptr;
	}

	Animations.clear();

}

void SpriteAnimator::Update(float dt)
{
	if (!currentAnimation) return;

	if (currentAnimation->doOnce)
	{
		if (ref->index == currentAnimation->animBounds.y -1 )
		{
			setAnimation(entryAnimation->name);
		}
	}
}

void SpriteAnimator::addAnimation(Animation* animation, bool isEntry)
{
	Animations.emplace_back(animation);
	animationMap[Animations.size() - 1] = animation;

	if (isEntry) {
		entryAnimation = animation;
	}

}

void SpriteAnimator::setAnimation(int index)
{
	Animation* anim = animationMap[index];
	anim->setAnimation(ref, nullptr);

	currentAnimation = anim;
	animationChange = true;
	currentAnimation->setAnimation(ref, nullptr);
}

void SpriteAnimator::setAnimation(std::string name)
{
	Animation* anim = getAnimation(name);
	if (!anim) return;
	currentAnimation = anim;
	animationChange = true;
	currentAnimation->setAnimation(ref, nullptr);
}

Animation* SpriteAnimator::getAnimation(int index) { return animationMap[index]; }

Animation* SpriteAnimator::getAnimation(std::string name)
{
	Animation* anim = nullptr;
	for (auto i : Animations)
	{
		if (i->name == name)
		{
			anim = i;
		}
	}
	return anim;
}

void Animation::Debug(const char* title)
{
	std::string titletorender = title ? title : name;

	if (ImGui::TreeNode(titletorender.c_str()))
	{
		ImGui::Checkbox("do once", &doOnce);
		ImGui::SliderInt2("limits", &animBounds.x, 0, 100);

		ImGui::TreePop();
	}
}

void SpriteAnimator::Debug(const char* title)
{
	if (ImGui::TreeNode(title))
	{
		if (animationRefFileExists)
		{
			if (ImGui::Button("Load Animations"))
			{
				LoadAnimationsFromFile_json(animationRefFile.c_str());
			}
			ImGui::SameLine();
			if (ImGui::Button("Clear Animations"))
			{
				clearAnimations();
			}
		}

		if (ImGui::TreeNode("Anim Scrubbing"))
		{
			ImGui::Checkbox("Animate enabled", &ref->animated);
			ImGui::SliderInt("start frame", &ref->frameStart, 0, 500);
			if (ImGui::SliderInt("frame Index", &ref->index, ref->frameStart, ref->frameEnd))
			{
				ref->spriteCoord = ref->getCoordinate(ref->index);
			}
			ImGui::SliderInt("end frame", &ref->frameEnd, 0, 500);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Animation list"))
		{
			for (auto i : Animations)
			{
				if (ImGui::TreeNode(i->name.c_str()))
				{

					i->Debug("Animation config");
					if (ImGui::Button("Set Animation"))
					{
						this->setAnimation(i->name);
					}
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}


#include "jsonParser.h"
Animation* ParseAnimation(const sajson::value& obj);

void SpriteAnimator::LoadAnimationsFromFile_json(const char* fileName)
{
	clearAnimations();
	char* data = nullptr;
	int size = 0;
	bool found = ReadFile(data, size, fileName);
	if (found == false)
	{
		printf("Could not load settings file...%s\n", fileName);
		return;
	}

	const sajson::document& document = sajson::parse(sajson::dynamic_allocation(), sajson::mutable_string_view(size, data));
	if (document.is_valid() == false)
	{
		int line = document.get_error_line();
		int column = document.get_error_column();
		const char* message = document.get_error_message_as_cstring();
		printf("Settings File (%s) is not valid\n\tLine %d, column %d:\t%s\n", fileName, line, column, message);
		return;
	}

	sajson::value root = document.get_root();
	int fieldCount = int(root.get_length());
	
	printf("fieldCount: %d\n", fieldCount);
	for (int i = 0; i < fieldCount; i++)
	{
		sajson::string fieldName = root.get_object_key(i);
		sajson::value fieldValue = root.get_object_value(i);
		//outputJsonContents(0, i, fieldName.as_string().c_str(), fieldValue, fieldValue.get_type());
		if (fieldName.as_string() == "meta")
		{
			int metaCount = int(fieldValue.get_length());
			printf("Meta-field Count: %d\n", metaCount);
			for (int ii = 0; ii < metaCount; ii++)
			{
				sajson::string name = fieldValue.get_object_key(ii);
				sajson::value value = fieldValue.get_object_value(ii);
				if (name.as_string() == "frameTags")
				{ //at this point value is of type array
					int frameTagCount = int(value.get_length());
					printf("frame-tag arr length: %d\n", frameTagCount);
					for (int iii = 0; iii < frameTagCount; iii++)
					{ //for each objeject pass the parse object function
						std::string animationname = "Animation";
						sajson::value tagValue = value.get_array_element(iii);
						Animation* newAnim = ParseAnimation(tagValue);
						if (newAnim)
						{
							this->addAnimation(newAnim, iii == 0);
						}
					}
				}
			}
		}
	}



}

Animation* ParseAnimation(const sajson::value& obj)
{
	if (obj.get_type() != sajson::type::TYPE_OBJECT) { printf("ParseAnimation:: obj is not of type Object!!!\n"); return nullptr; }

	int fieldCount = int(obj.get_length());
	glm::ivec2 bounds(0);
	std::string name = "Animation";
	bool singleUse = true;
	for (int i = 0; i < fieldCount; i++)
	{
		sajson::string FieldName = obj.get_object_key(i);
		sajson::value FieldValue = obj.get_object_value(i);

		if (FieldName.as_string() == "name" && FieldValue.get_type() == sajson::type::TYPE_STRING)
		{
			name = FieldValue.as_string();
		}

		if (FieldName.as_string() == "from" && FieldValue.get_type() == sajson::type::TYPE_INTEGER)
		{
			bounds.x = FieldValue.get_integer_value();
		}

		if (FieldName.as_string() == "to" && FieldValue.get_type() == sajson::type::TYPE_INTEGER)
		{
			bounds.y = FieldValue.get_integer_value();
		}

		if (FieldName.as_string() == "loop" && FieldValue.get_type() == sajson::type::TYPE_TRUE)
		{
			singleUse = false;
		}
		if (FieldName.as_string() == "loop" && FieldValue.get_type() == sajson::type::TYPE_FALSE)
		{
			singleUse = false;
		}
	}
	Animation* newAnim = new Animation(name, bounds, singleUse);
	//printf("Created Animation '%s : (%d - %d)'\n", newAnim->name.c_str(), newAnim->animBounds.x, newAnim->animBounds.y);
	return newAnim;
}



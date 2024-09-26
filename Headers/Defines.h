#pragma once

#define DECLARE_SINGLE(className)           \
private:                                    \
    className() {}                          \
public:                                     \
    static className* GetInstance()         \
    {                                       \
        static className s_instance;         \
        return &s_instance;                 \
    }                                       \

#define GET_SINGLE(className) className::GetInstance()

#define SAFE_DELETE(p)      \
    if (p)                  \
    {                       \
        delete p;           \
        p = nullptr;        \
    }                       \
     
#define ASSERT_CRASH(cause)                 \
{                                           \
    int32* p = nullptr;                     \
    if (!(cause))                           \
    {                                       \
        __analysis_assume(p != nullptr);    \
        *p = 0xDEADBEEF;                    \
    }                                       \
}                                           \


#define GET_DEVSCENE GET_SINGLE(SceneManager)->GetDevScene()
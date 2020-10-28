#pragma once
#include <functional>

#define NS_CC_BEGIN                     namespace cuddlycat {
#define NS_CC_END                       }
#define USING_NS_CC                     using namespace cuddlycat;
#define NS_CC                           ::cuddlycat

#define CC_CALLBACK_0(__selector__,__target__, ...) std::bind(&__selector__,__target__, ##__VA_ARGS__)
#define CC_CALLBACK_1(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)
#define CC_CALLBACK_2(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)
#define CC_CALLBACK_3(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, ##__VA_ARGS__)
#define CC_CALLBACK_4(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, ##__VA_ARGS__)


#if defined _DEBUG
#define GL_CHECK(x)						\
			{									\
				x;								\
				GLenum error = glGetError();	\
				assert(error == GL_NO_ERROR);	\
			}

#define CHECK_GL_ERROR_DEBUG() \
    do { \
        GLenum __error = glGetError(); \
        if(__error) { \
            printf("OpenGL error 0x%04X in %s %s %d\n", __error, __FILE__, __FUNCTION__, __LINE__); \
        } \
    } while (false)

#else
#define GL_CHECK(x) x;
#define CHECK_GL_ERROR_DEBUG()
#endif





NS_CC_BEGIN


typedef std::function<void (int, int, int)> MouseListener;
typedef std::function<void(double, double)> MouseMoveListener;
typedef MouseMoveListener MouseScrollListener;
typedef std::function<void(int, int, int, int)> KeyBoardListener;


NS_CC_END
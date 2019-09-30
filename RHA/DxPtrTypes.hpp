#pragma once
#include <wrl/client.h>

template<class T>
using DxPtr = Microsoft::WRL::ComPtr<T>;

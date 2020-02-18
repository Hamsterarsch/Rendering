#pragma once

#define DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(Typename)								\
		public: Typename() = default;														\
		public: Typename(const Typename &Other) = default;									\
		public: Typename(Typename &&Other) = default;										\
		public: virtual ~Typename() = default;												\
																							\
		public: Typename &operator=(const Typename &Other) = default;						\
		public: Typename &operator=(Typename &&Other) = default;


#define DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS_NOCTOR(Typename)						\
		public: Typename(const Typename &Other) = default;									\
		public: Typename(Typename &&Other) = default;										\
		public: virtual ~Typename() = default;												\
																							\
		public: Typename &operator=(const Typename &Other) = default;						\
		public: Typename &operator=(Typename &&Other) = default;
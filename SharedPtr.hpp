#ifndef SHARED_HPP_
#define SHARED_HPP_
#include <thread>
namespace cs540 {

class Reference{
public:
	int refCount;
	Reference() : refCount(0){
}
    	int increRef(){
	
		refCount++;
		return refCount;
    	}
int decreRef(){

		if(refCount > 0) {
			refCount--;
			
		}
if(refCount == 0) Delete();

    	return refCount;
}
virtual ~Reference(){ if(refCount==0)Delete(); }
virtual void Delete(){}
};


template <typename U>
class help_shared_ptr: public Reference{
public:
	U* helper;
help_shared_ptr(U* p): helper(p){

}
	
virtual void Delete(){
    if(helper!=nullptr){
	delete helper;
	helper = nullptr;
	}
}
~help_shared_ptr(){

	if(helper != nullptr)
	delete helper;
}
};

template <typename T>
class SharedPtr{
public:
	T* shared_ptr;
    	Reference* counter;


explicit operator bool() const{ return shared_ptr != nullptr; }


T *get() const{	return shared_ptr;    	}


T& operator*() const{  return *shared_ptr;  }


T *operator->() const{ return shared_ptr;   }

    	
SharedPtr() : shared_ptr(nullptr), counter(nullptr){}


template <typename U> explicit SharedPtr(U *p){
	
	counter = new help_shared_ptr<U>(p);
	counter->increRef();
	shared_ptr = p;
}

SharedPtr(const SharedPtr& sp){

	shared_ptr = sp.shared_ptr;
	counter = sp.counter;
	if(counter != nullptr)
	{counter->increRef();}
}


template <typename U> SharedPtr(const SharedPtr<U>& sp){

	shared_ptr = sp.shared_ptr;
	counter = sp.counter;
	if(counter != nullptr)
		counter->increRef();
   	}


SharedPtr(SharedPtr&& sp){

	shared_ptr = sp.shared_ptr;
	counter = sp.counter;
	sp.counter = nullptr;
	sp.shared_ptr = nullptr;
}


template <typename U> SharedPtr(SharedPtr<U>&& sp){
	if(shared_ptr != sp.shared_ptr){
    	reset(nullptr);
	shared_ptr = sp.shared_ptr;
    	counter = sp.counter;
	sp.shared_ptr = nullptr;
	sp.counter = nullptr;
	}
}


~SharedPtr(){

	if(shared_ptr != nullptr){
	shared_ptr = nullptr;
	if(counter != nullptr) {
    		int count = counter->decreRef();
			if(count == 0){
				delete counter;
			}
			counter = nullptr;
		}
       	}

}

void reset() {
std::this_thread::sleep_for(std::chrono::milliseconds(20));

if(shared_ptr != nullptr){
shared_ptr = nullptr;
     if(counter != nullptr) {
	int refCount = counter->decreRef();
	if(refCount == 0){
	  delete counter;
	}
	counter = nullptr;
	}
     }
}


template <typename U>
void reset(U * p){
std::this_thread::sleep_for(std::chrono::milliseconds(20));
if(shared_ptr != nullptr){
	shared_ptr = nullptr;
	if(counter != nullptr) {
   		int Count = counter->decreRef();
		if(Count == 0){
			delete counter;
		}
		counter = nullptr;
	}
  }
std::this_thread::sleep_for(std::chrono::milliseconds(20));
counter = new help_shared_ptr<U>(p);
counter->increRef();
shared_ptr = p;
}


SharedPtr& operator=(SharedPtr&& sp){
if(shared_ptr != nullptr){
 	shared_ptr = nullptr;
		if(counter != nullptr) {
    		int refCount = counter->decreRef();
		if(refCount == 0){
			delete counter;
		}
		counter = nullptr;
	}
      	}
shared_ptr = sp.shared_ptr;
counter = sp.counter;
sp.counter = nullptr;
sp = nullptr;
return *this;
}


template <typename U> SharedPtr& operator=(SharedPtr<U>&& sp) throw(){
std::this_thread::sleep_for(std::chrono::milliseconds(10));
if(shared_ptr != nullptr){
shared_ptr = nullptr;
	if(counter != nullptr) {
    		int refCount = counter->decreRef();
		if(refCount == 0){
			delete counter;
std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		counter = nullptr;
	}
   	}
shared_ptr = sp.shared_ptr;
counter=sp.counter;
sp.counter = nullptr;
sp.shared_ptr = nullptr;
return *this;
}


SharedPtr& operator=(const SharedPtr& sp){
if(shared_ptr != nullptr){
shared_ptr = nullptr;
	if(counter != nullptr) {
	int refCount = counter->decreRef();
	if(refCount == 0){
		delete counter;
	}
	counter = nullptr;
	}
}
shared_ptr = sp.shared_ptr;
counter = sp.counter;
if(counter != nullptr)
	counter->increRef();
return *this;
}


template <typename U> SharedPtr& operator=(SharedPtr<U>& sp){
	if(shared_ptr != nullptr){
	shared_ptr = nullptr;
	if(counter != nullptr) {
  		int refCount = counter->decreRef();
		if(refCount == 0){
			delete counter;
		}
		counter = nullptr;
	}
      	}
shared_ptr = sp.shared_ptr;
counter = sp.counter;
if(counter != nullptr)
	counter->increRef();
return *this;
}

};

template <typename T1, typename T2>
bool operator==(const SharedPtr<T1> &a, const SharedPtr<T2> &b) {
   return ( a.get() == b.get() );
   return (a.get()==nullptr && b.get()==nullptr);
}

template <typename T>
bool operator==(const SharedPtr<T> &a, std::nullptr_t) {
   return a.get()==nullptr;
}

template <typename T>
bool operator==(std::nullptr_t, const SharedPtr<T> &a) {
  return a.get()==nullptr;
}

template <typename T1, typename T2>
bool operator!=(const SharedPtr<T1>&a, const SharedPtr<T2> &b){
 return ( a.get() != b.get() );
}

template <typename T>
bool operator!=(const SharedPtr<T> &a, std::nullptr_t){
  return a.get()!=nullptr;
}

template <typename T>
bool operator!=(std::nullptr_t, const SharedPtr<T> &a){
  return a.get()!=nullptr;
}

//This portion of code is not working
template <typename T, typename U>
SharedPtr<T> static_pointer_cast(const SharedPtr<U> &sp){
return SharedPtr<T>(static_cast<T*>(sp.get()));
}

// yet to verify if this code works fine. Tried but it fails test case
template <typename T, typename U>
SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U> &sp){
if(T* k=dynamic_cast<T*>(sp.get()) )
return SharedPtr<T>(k);
return SharedPtr<T>();
}
}
#endif 

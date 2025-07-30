#pragma once
#include<iostream>
#include<type_traits>
#include<cstdlib>
#include<initializer_list>
#include<utility>
#include<new>
#include<functional>
#include<algorithm>
#include"Macros.h"
//interface:
//emplace,top,push,empty,pop,size,swap,show
//
_PANAGIOTIS_BEGIN
template<typename _Ty>
class stack final {
private:
	class stack_node final{
	private:
		class secret_class {};
		template<class ..._Valty>
		stack_node(secret_class,_Valty&&..._Val) :data{ std::forward<_Valty>(_Val)... }, next{ nullptr }
		{
		}
	public:
		stack_node* next;
		_Ty data;
		stack_node()
			:data{}, next{ nullptr }
		{
			static_assert(std::is_default_constructible_v<_Ty>, "the type must be default constructible");
		}
		stack_node(const _Ty& item)noexcept(noexcept(data=item)):
			next{ nullptr }
		{
			static_assert(std::is_copy_assignable_v<_Ty>, "the type must be copy assignable");
			data = item;
		}
		stack_node(_Ty&& item) noexcept(noexcept(data=std::move(item)))
			:next{nullptr}
		{
			static_assert(std::is_move_assignable_v<_Ty>, "the type must be move assignable");
			data = std::move(item);
		}
		template<class ..._Valty>
		static stack_node* craft(_Valty&& ..._Val) {

			stack_node* ptr = new(std::nothrow) stack_node{ secret_class{},std::forward<_Valty>(_Val)... };
			return ptr;

		}
	};
	std::size_t count;
	stack_node* head; 
	template<typename _Valty>
	bool push_node(_Valty &&_Val) {
		stack_node* ptr = new (std::nothrow)stack_node(std::forward<_Valty>(_Val));
		if (ptr != nullptr) {
			ptr->next = head;
			head = ptr;
			count++;
			return true;
			
		}
		else {
			return false;
		}
	}
	void pop_node() 
	{
		static_assert(std::is_destructible_v<_Ty>, "the type must be destructbile");
		if (count != 0) {
			stack_node* ptr = head->next;
			delete head;
			head = ptr;
			count--;
			return;
		}
		throw pop_from_empty_stack_{ "tried to pop from an empty stack\n" };
	}
	template<typename ..._Valty>
	bool emplace_node(_Valty&&..._Val) {
		stack_node* ptr = stack_node::craft(std::forward<_Valty>(_Val)...);
		if (ptr != nullptr) {
			ptr->next = head;
			head = ptr;
			count++;
			return true;
		}
		else {
			return false;
		}
	}
	void clear()noexcept {
		static_assert(std::is_destructible_v<_Ty>, "the type must be destructbile");
		stack_node* ptr{};
		for (size_t i = 0; i < count; i++) {
			ptr = head;
			head = head->next;
			delete ptr;
		}
		head = nullptr;
		count = 0;
	}
public:
	//some type traits in order to use the container 
	static_assert(std::is_object_v<_Ty>, "The C++ Standard forbids container adaptors of non-object types "
		"because of [container.requirements].");
	static_assert(!std::is_reference_v<_Ty>, "no references allowed");
	static_assert(!std::is_const_v<_Ty>, "no const types are allowed");
	static_assert(!std::is_volatile_v<_Ty>, "no volatile types are allowed");
	static_assert(!std::is_array_v<_Ty>, "no raw c arrays are allowed");

	stack()noexcept :head{ nullptr }, count{}
	{

	}
	stack(const stack<_Ty>& other) :head{ nullptr }, count{} {
		static_assert(std::is_constructible_v<stack_node, _Ty>,"the type must be constructible");
		if (this != &other) {
			stack_node* ptr = other.head;
			if (ptr == nullptr)return;
			head = new (std::nothrow)stack_node(ptr->data);
			if (head == nullptr)return;
			count++;
			stack_node* curr{ head };
			ptr = ptr->next;
			while (ptr != nullptr) {
				curr->next = new (std::nothrow)stack_node(ptr->data);
				if (curr->next != nullptr) {
					curr = curr->next;
					ptr = ptr->next;
					count++;
				}
				else {
					clear();
					break;
				}
			}
		}
	}
	stack(const std::initializer_list<_Ty>& l) :head{ nullptr }, count{}
	{
		const _Ty* b = l.begin();
		for (std::size_t i = 0; i < l.size(); i++) {
			if (push(*b)) {
				b++;
			}
			else {
				clear();
				break;
			}

		}
	}
	stack(stack<_Ty>&& other)noexcept :head{ nullptr }, count{} {
		std::swap(head, other.head);
		std::swap(count, other.count);
	}

	bool push(const _Ty&data) 
	{
		return push_node(data);

	}
	bool push(_Ty&& data) { 
		return push_node(std::move(data));
	}

	_NODISCARD std::size_t size()const noexcept {
		return count;
	}
	_NODISCARD bool empty()const noexcept {
		return count == 0;
	}
	void pop() { 
		pop_node();
		
	}
	_NODISCARD _Ty&&  top()&&
	{
		if (count == 0) {
			throw bad_stack_access_{ "trying to access an empty stack\n" };
		}
		return std::move(head->data);
	}
	_NODISCARD const _Ty&& top()const&&
	{
		if (count == 0) {
			throw bad_stack_access_{ "trying to access an empty stack\n" };
		}
		return std::move(head->data);
	}
	_NODISCARD _Ty& top()&
	{
		if (count == 0) {
			throw bad_stack_access_{ "trying to access an empty stack\n" };
		}
		return head->data;
	}
	_NODISCARD const _Ty& top()const&
	{
		if (count == 0) {
			throw bad_stack_access_{ "trying to access an empty stack\n" };
		}
		return head->data;
	}

	template<class ..._Valty>
	bool emplace(_Valty&&..._Val) {
		return emplace_node(std::forward<_Valty>(_Val)...);
		
	}
	void swap(stack<_Ty> &other)noexcept {
		std::swap(head, other.head);
		std::swap(count, other.count);
	}

	void show() {
		stack_node* ptr{ head };
		while (ptr != nullptr) {
			std::cout << ptr->data << '\n';
			ptr = ptr->next;
		}
	}
	~stack() noexcept{
		clear();
		
	}
	stack<_Ty>& operator =(const stack<_Ty>& other)& {
		static_assert(std::is_destructible_v<_Ty>, "the type must be destructbile");
		stack_node* curr1{head};
		stack_node* curr2{other.head};
		stack_node* prev1{ nullptr };
		stack_node* prev2{ nullptr };
		while (curr1 != nullptr && curr2 != nullptr) {
			curr1->data = curr2->data;
			prev1= curr1;
			prev2= curr2;
			curr1 = curr1->next;
			curr2 = curr2->next;
			
		}
		if (prev1 == nullptr  && curr2!=nullptr) {//protos adios
			head = new(std::nothrow) stack_node(curr2->data);
			if (head == nullptr)return *this;
			count++;
			curr1 = head;
			curr2 = curr2->next;
			while (curr2 != nullptr) {
				curr1->next = new (std::nothrow)stack_node(curr2->data);
				if (curr1->next != nullptr) {
					curr1 = curr1->next;
					curr2 = curr2->next;
					count++;
				}
				else {
					clear();
					break;
				}
			}
			return *this;
			
		}
		if (prev2 == nullptr && curr1 != nullptr) {
			clear();
			return *this;
		}
		//if curr1==curr2==nullptr both have same size
		if (curr1 != nullptr) {
			prev1->next = nullptr;
			while (curr1 != nullptr) {//size>other.count,delete extra
				prev1 = curr1;
				curr1 = curr1->next;
				delete prev1;
				count--;
			}
			return *this;
		}
		if (curr2 != nullptr) {//size<other.count,allocate new nodes
			while (curr2 != nullptr) {
				prev1->next = new(std::nothrow) stack_node(curr2->data);
				if (prev1->next != nullptr) {
					prev1 = prev1->next;
					curr2 = curr2->next;
					count++;
				}
				else {
					clear();
					break;
				}
				
			}
			return *this;
		}
		return *this;
	}
	stack<_Ty>& operator =(stack<_Ty>&& other) &noexcept {
		clear();
		std::swap(head, other.head);
		std::swap(count, other.count);
		
		return *this;
	}
	_NODISCARD std::size_t count_items()const noexcept {
		stack_node* ptr{ head };
		std::size_t node_count{ 0 };
		while (ptr != nullptr) {
			node_count++;
			ptr = ptr->next;
		}
		return node_count;
	}



};

_PANAGIOTIS_END

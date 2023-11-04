//
// Created by Steve on 04.11.2023.
//

#include "pch.h"

#include "tqueue.h"

#include <thread>

using namespace utl;

TEST(TQueue, Empty) {
	TQueue<int> queue;

	auto t1 = std::thread([&]() { ASSERT_TRUE(queue.empty()); });
	auto t2 = std::thread([&]() { ASSERT_TRUE(queue.empty()); });

	t1.join();
	t2.join();
}

TEST(TQueue, PushAndEmpty) {
	TQueue<int> queue;

	auto t1 = std::thread([&]() { queue.push(42); });
	auto t2 = std::thread([&]() { ASSERT_FALSE(queue.empty()); });

	t1.join();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	t2.join();
}

TEST(TQueue, PopEmptyAndStop) {
	TQueue<int> queue;

	auto t1 = std::thread([&]() { queue.pop(); });
	auto t2 = std::thread([&]() { ASSERT_TRUE(queue.empty()); });
	auto t3 = std::thread([&]() { queue.stop(); });

	t1.join();
	t2.join();
	t3.join();
}

TEST(TQueue, PushAndPop) {
	TQueue<int> queue;

	queue.push(42);

	ASSERT_EQ(queue.pop(), 42);
}

TEST(TQueue, PopAndPush) {
	TQueue<int> queue;

	auto t1 = std::thread([&]() { ASSERT_EQ(queue.pop(), 42); });
	auto t2 = std::thread([&]() { queue.push(42); });

	t1.join();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	t2.join();
}

TEST(TQueue, PopAndStop) {
	TQueue<int> queue;

	auto t1 = std::thread([&]() { ASSERT_EQ(queue.pop(), std::nullopt); });
	auto t2 = std::thread([&]() { queue.stop(); });

	t1.join();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	t2.join();
}

int assign_copy_counter{0};

struct CopyHelper {
	explicit CopyHelper(int& copy_counter) : ctor_copy_counter{copy_counter} {
	}
	CopyHelper(const CopyHelper& o) : ctor_copy_counter{o.ctor_copy_counter} {
		ctor_copy_counter++;
	}
	CopyHelper& operator=(const CopyHelper& o) {
		assign_copy_counter++;
		return *this;
	}

	int& ctor_copy_counter;
};

TEST(TQueue, PushLValueNoCopy) {
	TQueue<CopyHelper> queue;
	int ctor_copy_counter{0};

	CopyHelper c{ctor_copy_counter};

	ASSERT_EQ(ctor_copy_counter, 0);

	queue.push(std::move(c));

	ASSERT_EQ(ctor_copy_counter, 1);
}

TEST(TQueue, PushRValueNoCopy) {
	TQueue<CopyHelper> queue;
	int ctor_copy_counter{0};

	ASSERT_EQ(ctor_copy_counter, 0);

	queue.push(CopyHelper(ctor_copy_counter));

	ASSERT_EQ(ctor_copy_counter, 1);
}

TEST(TQueue, PushContainerNoCopy) {
	TQueue<std::vector<CopyHelper>> queue;
	int ctor_copy_counter{0};
	std::vector<CopyHelper> vec{CopyHelper{ctor_copy_counter}};

	ASSERT_EQ(ctor_copy_counter, 1);

	queue.push(std::move(vec));

	ASSERT_EQ(ctor_copy_counter, 1);
}

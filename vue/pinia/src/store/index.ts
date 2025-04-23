import { defineStore } from 'pinia'
import { IDS } from './enum'
import type { User } from '../types'

const testUser: User = {
  name: 'just a test user',
  age: 18
}

const asynctestUser = () => new Promise<User>((resolve) => {
  setTimeout(() => {
    resolve({
      name: 'a async server',
      age: 19
    })
  }, 2000)
})

const longLiveData = 'longLiveData'

export const useTestStore = defineStore(IDS.TEST, {
  // 存储核心数据的地方
  state: () => {
    return {
      current: 1,
      name: 'chulan',
      user: {} as User,
      longLiveData: ''
    }
  },

  // 计算属性
  getters: {
    // 将state传入
    nameChanged: (state) => state.name + 'changed'
  },

  // 修改核心数据，支持我们进行同步和异步操作
  actions: {
    changedName() {
      this.name = 'func1'
    },
    setUser() {
      this.user = testUser
    },
    async asyncsetUser() {
      const result = await asynctestUser()
      this.user = result
    },
    // 持久化数据
    setLongLiveData() {
      this.longLiveData = longLiveData
    }
  }
})

export const useBaseStore = defineStore(IDS.BASE, {
  state: () => {
    return {
      baseData: 'baseData'
    }
  },
  getters: {
    baseDataChanged: (state) => state.baseData + 'changed'
  },
  actions: {
    changedBaseData() {
      this.baseData = 'func1, ' + this.baseData
    }
  }
})
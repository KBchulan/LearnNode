// expire过期时间，permanent则是设置为永久
var Dictionaries;
(function (Dictionaries) {
    Dictionaries["permanent"] = "permanent";
    Dictionaries["expire"] = "__expire__";
})(Dictionaries || (Dictionaries = {}));

class Storage {
    set(key, value, expire = Dictionaries.permanent) {
        const data = {
            value,
            [Dictionaries.expire]: expire
        };
        localStorage.setItem(key, JSON.stringify(data));
    }
    get(key) {
        const value = localStorage.getItem(key);
        if (value) {
            const data = JSON.parse(value);
            const now = new Date().getTime();
            if (typeof data[Dictionaries.expire] == 'number'
                && data[Dictionaries.expire] < now) {
                this.remove(key);
                return {
                    message: `您的${key}已过期`,
                    value: null
                };
            }
            else {
                return {
                    message: "成功",
                    value: data.value
                };
            }
        }
        else {
            return {
                message: '值无效',
                value: null
            };
        }
    }
    remove(key) {
        localStorage.removeItem(key);
    }
    clear() {
        localStorage.clear();
    }
}

export { Storage };

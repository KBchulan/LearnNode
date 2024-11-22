import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.basemap import Basemap

def to_cat_list(catstr):
    if pd.isna(catstr):
        return []
    stripped = (x.strip() for x in str(catstr).split(','))
    return [x for x in stripped if x]

def get_all_categories(cat_series):
    cat_sets = (set(to_cat_list(x)) for x in cat_series)
    return sorted(set.union(*cat_sets))

def get_english(cat):
    code, names = cat.split('.')
    if '|' in names:
        names = names.split('|')[1]
    return code, names.strip()

def get_code(seq):
    return [x.split('.')[0] for x in seq if x]

def basic_haiti_map(ax=None, lllat=17.25, urlat=20.25, lllon=-75, urlon=-71):
    m = Basemap(ax=ax, 
                projection='stere',
                lon_0=(urlon + lllon)/2,
                lat_0=(urlat + lllat)/2,
                llcrnrlat=lllat, 
                urcrnrlat=urlat,
                llcrnrlon=lllon, 
                urcrnrlon=urlon,
                resolution='l')  # 改为 'l' (low) 而不是 'f' (full)
    
    m.drawcoastlines()
    m.drawstates()
    m.drawcountries()
    return m

data = pd.read_csv('haiti/Haiti.csv')
data['CATEGORY'] = data['CATEGORY'].fillna('')

all_cats = get_all_categories(data.CATEGORY)
english_mapping = dict(get_english(x) for x in all_cats)
all_codes = get_code(all_cats)
code_index = pd.Index(np.unique(all_codes))

dummy_frame = pd.DataFrame(np.zeros((len(data), len(code_index))),
                         index=data.index,
                         columns=code_index)

for row, cat in zip(data.index, data.CATEGORY):
    codes = get_code(to_cat_list(cat))
    dummy_frame.loc[row, codes] = 1

data = data.join(dummy_frame.add_prefix('category_'))

fig, axes = plt.subplots(nrows=2, ncols=2, figsize=(12, 10))
fig.subplots_adjust(hspace=0.05, wspace=0.05)

to_plot = ['2a', '1', '3c', '7a']
for code, ax in zip(to_plot, axes.flat):
    m = basic_haiti_map(ax)
    cat_data = data[data[f'category_{code}'] == 1]
    x, y = m(cat_data.LONGITUDE.values, cat_data.LATITUDE.values)
    m.plot(x, y, 'k.', alpha=0.5)
    ax.set_title(f'{code}: {english_mapping[code]}')

plt.show()
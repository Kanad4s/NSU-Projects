import pandas as pd
from matplotlib import pyplot as plt
import seaborn as sns
import numpy as np

data_set = pd.read_csv('../base/Laptop_price.csv')
print(data_set.describe())

duplicates_count = data_set.duplicated().sum()
print(f"Количество дубликатов: {duplicates_count}")
for i in range(len(data_set.columns)):
    duplicates = data_set[data_set.columns[i]].duplicated().sum()
    print(f"дубликатов {data_set.columns[i]}: {duplicates}")

sns.heatmap(data_set.isnull(),yticklabels=False,cbar=False,cmap='viridis')

tmp = data_set.copy()
tmp.drop_duplicates(inplace=True)
dfNumerical = tmp.select_dtypes(include = [np.number])
dfCategorical = tmp.select_dtypes(exclude = [np.number])
print(dfNumerical.describe())
print(dfCategorical.describe())
print(dfNumerical.nunique())
print(dfCategorical.nunique())

from sklearn.preprocessing import OneHotEncoder, LabelEncoder
encoder = LabelEncoder()
encoderData = encoder.fit_transform(dfCategorical.values)
encoderDF = pd.DataFrame(encoderData, columns=['Brand'])
# da = pd.DataFrame(data_new.toarray(), columns=encoder.categories_)
# print(encoderDF.describe())
# print(encoder.classes_)

cleanData = pd.concat([dfNumerical, encoderDF], axis=1)
# print(cleanData.describe())

baseDF = cleanData.copy()
# baseDF = data_set.copy()
df = pd.DataFrame()
dfLength = len(baseDF)
# print(f'data_set length: {dfLength}')
for i in range(len(baseDF.columns)):
    if baseDF[baseDF.columns[i]].dtype != np.float_:
      df.insert(i, baseDF.columns[i], baseDF[baseDF.columns[i]])
    else:
      df.insert(i, baseDF.columns[i], np.arange(dfLength, dtype='int64'))

def discretizeFloatToInt(out, y, column):
  nStep = 5
  ma = max(y)
  mi = min(y)
  step = (ma - mi) / nStep
  # print(f'max: {max(y)}, min: {min(y)}, step: {step}')
  for i in range(len(y)):
    for idx in range(1, nStep + 1):
      if y[i] <= mi + idx * step:
        # print(f"y[i] = {y[i]}, [{mi + (idx - 1) * step}, {mi + idx * step}], index: {idx}")
        out.at[i, column] = idx
        break

for i in range(len(baseDF.columns)):
    # print(baseDF[baseDF.columns[i]].dtype)
    if baseDF[baseDF.columns[i]].dtype == np.float64:
        # print("discretized")
        discretizeFloatToInt(df, baseDF[baseDF.columns[i]], baseDF.columns[i])
# print(df.describe())
# print(df)

for column in df.columns:
    unique_values = df[column].unique()
    print(f"{column}: {unique_values}")

# Entropy 
def entropy(y):
  counts = np.bincount(y)
  probabilities = counts / len(y)
  return -np.sum([p * np.log2(p) for p in probabilities if p > 0])

# Information Gain (IG)
def informationGain(dataSet, featureClass, featureIdx):
  totalEntropy = entropy(featureClass)
  values, counts = np.unique(dataSet[:, featureIdx], return_counts=True)
  weightedEntropy = 0

  for value, count in zip(values, counts):
      subset = featureClass[dataSet[:, featureIdx] == value]
      weightedEntropy += (count / len(dataSet)) * entropy(subset)

  return totalEntropy - weightedEntropy

# Split Information (SI)
def splitInformation(dataSet, featureIdx):
    values, counts = np.unique(dataSet[:, featureIdx], return_counts=True)
    probabilities = counts / len(dataSet)
    return -np.sum([p * np.log2(p) for p in probabilities if p > 0])

# Gain Ratio
def gainRatio(dataSet, featureClass, featureIdx):
    ig = informationGain(dataSet, featureClass, featureIdx)
    iv = splitInformation(dataSet, featureIdx)
    return ig / iv if iv != 0 else 0

# my Gain Ratio
ds = df.drop(['Price'], axis=1)
columns = ds.columns.tolist()
ds = ds.values
y = df['Price'].values
scores = [gainRatio(ds, y, i) for i in range(ds.shape[1])]

ranking = np.argsort(scores)[::-1]

dfgr = pd.DataFrame()

print("Gain Ratio:")
for i in ranking:
    print(f"{columns[i]}: {scores[i]:.5f}")
    dfgr[columns[i]] = [scores[i]]

print(dfgr)

# sklearn
from sklearn.feature_selection import SelectKBest, f_classif, chi2, VarianceThreshold
# x = df.drop(['Brand', 'Price'], axis=1)
x = df.drop(['Price'], axis=1)
y = df['Price']
print(x.describe())
tmpSelector = VarianceThreshold()
X_train_filtered = tmpSelector.fit_transform(x)
kbest = SelectKBest(score_func = chi2, k = 'all')
kbest.fit(X_train_filtered, y)

scores = kbest.scores_
feature_names = x.columns
sorted_indices = np.argsort(scores)[::-1]
sorted_scores = scores[sorted_indices]
sorted_features = [feature_names[i] for i in sorted_indices]

print("Признаки в порядке значимости:")
for feature, score in zip(sorted_features, sorted_scores):
    print(f"{feature}: {score:.2f}")
    dfgr.loc[1, feature] = score

print(dfgr)
dfgr.to_csv("../base/gr_laptop.csv")
df.to_csv("../base/processed_laptop.csv", index=False)

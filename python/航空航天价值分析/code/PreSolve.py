import pandas as pd
from datetime import datetime

# ...Data Preprocessor...
class DataPreprocessor:
    def __init__(self, file_path = "../resources/raw_data.csv"):
        self.raw_data = pd.read_csv(file_path , encoding='gb18030')
        self.processed_data = None
        self.lrfmc_data = None
        
    def clean_data(self):
        self.processed_data = self.raw_data.copy()
        
        self.processed_data = self.processed_data[(self.processed_data['SUM_YR_1'] != 0) | (self.processed_data['SUM_YR_2'] != 0)]

        self.processed_data = self.processed_data[self.processed_data['avg_discount'] != 0]
        
        self.processed_data = self.processed_data[self.processed_data['SEG_KM_SUM'] > 0]
        
        return self
        
    def create_lrfmc_features(self):
        end_date = datetime.strptime('2014/3/31', '%Y/%m/%d')

        self.lrfmc_data = pd.DataFrame({
            'L': (end_date - pd.to_datetime(self.processed_data['FFP_DATE'], format = 'mixed', errors = 'coerce')).dt.days / 30,
            'R': (end_date - pd.to_datetime(self.processed_data['LAST_FLIGHT_DATE'], format = 'mixed', errors = 'coerce')).dt.days / 30,
            'F': self.processed_data['FLIGHT_COUNT'],
            'M': self.processed_data['SEG_KM_SUM'],
            'C': self.processed_data['avg_discount']
        })

        self.lrfmc_data['L'] = self.lrfmc_data['L'].fillna(self.lrfmc_data['L'].mean())
        self.lrfmc_data['R'] = self.lrfmc_data['R'].fillna(self.lrfmc_data['R'].mean())
        
        for column in self.lrfmc_data.columns:
            self.lrfmc_data[column] = self.lrfmc_data[column].fillna(self.lrfmc_data[column].mean())

        return self
    
    def standardize_features(self):            
        for column in self.lrfmc_data.columns:
            mean = self.lrfmc_data[column].mean()
            std = self.lrfmc_data[column].std()
            self.lrfmc_data[column] = (self.lrfmc_data[column] - mean) / std
            
        return self
    
    def save_processed_data(self, output_path):            
        self.lrfmc_data.to_csv(output_path, index = False)
    

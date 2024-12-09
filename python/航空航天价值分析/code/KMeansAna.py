import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

from sklearn.cluster import KMeans
from PreSolve import DataPreprocessor

class CustomerSegmentation:
    def __init__(self, data_path, n_clusters = 5):
        self.data = pd.read_csv(data_path)
        
        for column in self.data.columns:
            self.data[column] = self.data[column].fillna(self.data[column].mean())
        
        self.n_clusters = n_clusters
        self.kmeans = None
        self.labels = None
        self.cluster_centers = None
        
    def perform_clustering(self):
        self.kmeans = KMeans(
            n_clusters = self.n_clusters,
            init = 'k-means++',
            n_init = 10,
            max_iter = 300,
            tol = 0.0001,
            random_state = 42
        )
        
        self.labels = self.kmeans.fit_predict(self.data)
        self.cluster_centers = self.kmeans.cluster_centers_
        return self
        
    def analyze_clusters(self):
        result_df = self.data.copy()
        result_df['Cluster'] = self.labels
        
        cluster_sizes = result_df['Cluster'].value_counts().sort_index()
        
        cluster_summary = pd.DataFrame({
            'Cluster': range(self.n_clusters),
            'Size': cluster_sizes.values,
            'ZL': self.cluster_centers[:, 0],
            'ZR': self.cluster_centers[:, 1],
            'ZF': self.cluster_centers[:, 2],
            'ZM': self.cluster_centers[:, 3],
            'ZC': self.cluster_centers[:, 4]
        })
        
        return cluster_summary
    
    def plot_radar_chart(self, cluster_summary):
        categories = ['L', 'R', 'F', 'M', 'C']
        angles = np.linspace(0, 2 * np.pi, len(categories), endpoint = False)
        angles = np.concatenate((angles, [angles[0]]))
        
        fig, ax = plt.subplots(figsize = (10, 10), subplot_kw = dict(projection = 'polar'))
        
        circles = [-2.0, -1.5, -1.0, -0.5, 0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0]
        for circle in circles:
            ax.plot(angles, [circle]*len(angles), 'gray', linestyle = '--', alpha = 0.2)
        
        line_styles = ['-', '--', ':', '-.', '--']
        colors = ['black', 'red', 'green', 'blue', 'cyan']
        
        for i in range(self.n_clusters):
            values = cluster_summary.iloc[i, 2:].values
            values = np.clip(values, -2.0, 3.0)
            values = np.concatenate((values, [values[0]]))
            ax.plot(angles, values, linestyle = line_styles[i], color = colors[i], 
                   linewidth = 1.5, label = f'Group {i+1}')
        
        ax.set_xticks(angles[:-1])
        ax.set_xticklabels(categories, fontsize = 12)
        
        ax.set_rlabel_position(0)
        ax.grid(False)
        
        ax.set_ylim(-2.0, 3.0)
        ax.set_rticks(circles)
        ax.set_yticklabels([str(x) for x in circles], fontsize = 8)
        
        plt.legend(loc = 'center right', bbox_to_anchor = (1.3, 0.5))
        
        ax.spines['polar'].set_visible(True)
        
        plt.tight_layout()
        plt.savefig('../resources/res.png', dpi = 300, bbox_inches = 'tight')
        plt.close()
    
    def analyze_customer_characteristics(self, cluster_summary):
        characteristics = []
        
        for i in range(self.n_clusters):
            cluster = cluster_summary.iloc[i]
            
            features = {
                'L': cluster['ZL'],
                'R': cluster['ZR'],
                'F': cluster['ZF'],
                'M': cluster['ZM'],
                'C': cluster['ZC']
            }
            
            sorted_features = sorted(features.items(), key = lambda x: abs(x[1]), reverse = True)
            strong_features = [k for k, v in sorted_features if v > 0.5]
            weak_features = [k for k, v in sorted_features if v < -0.5]
            
            characteristics.append({
                'cluster': i,
                'size': cluster['Size'],
                'strong_features': strong_features,
                'weak_features': weak_features
            })
            
        return characteristics

def main():
    pre = DataPreprocessor("../resources/raw_data.csv")
    pre.clean_data().create_lrfmc_features().handle_missing_values().standardize_features()
    pre.save_processed_data("../resources/processed_data.csv")
    
    segmentation = CustomerSegmentation("../resources/processed_data.csv")
    segmentation.perform_clustering()
    
    cluster_summary = segmentation.analyze_clusters()
    print("聚类结果汇总:")
    print(cluster_summary)
    
    segmentation.plot_radar_chart(cluster_summary)
    
    characteristics = segmentation.analyze_customer_characteristics(cluster_summary)
    
    print("客户群特征分析:\n")
    for char in characteristics:
        print(f"客户群 {char['cluster']+1} (数量: {char['size']}):")
        print(f"优势特征: {', '.join(char['strong_features'])}")
        print(f"劣势特征: {', '.join(char['weak_features'])}")

if __name__ == "__main__":
    main() 
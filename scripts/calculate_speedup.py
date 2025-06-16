import pandas as pd
import argparse

def calculate_speedup(file1, file2, output_file):
    df1 = pd.read_csv(file1)
    df2 = pd.read_csv(file2)

    req_columns = ['Width', 'Samples', 'Depth' , 'NumberGrid', 'Time']
    for col in req_columns:
        if col not in df1.columns or col not in df2.columns:
            raise ValueError(f"Both CSV files most contain the column: {col}")

    merge_columns = ['Width', 'Samples', 'Depth', 'NumberGrid']
    merged_df = pd.merge(df1, df2, on=merge_columns, suffixes=('_file1', '_file2'))

    merged_df['Speedup'] = merged_df['Time_file1'] / merged_df['Time_file2']

    output_df = merged_df[merge_columns + ['Speedup']]

    output_df.to_csv(output_file, index=False)
    print(f"Speedup data written to {output_file}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Calculate speed up")
    parser.add_argument("--cpu_file", help="Path to CPU .csv file")
    parser.add_argument("--cuda_file", help="Path to CUDA .csv file")
    parser.add_argument("--output_file", help="Path to ouptut .csv file")
    args = parser.parse_args()
    calculate_speedup(args.cpu_file, args.cuda_file, args.output_file)

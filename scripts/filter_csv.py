import pandas as pd
import argparse

# input arguments: input file, file to path and common name, Width, Samples, number of spheres (list)

CLI = argparse.ArgumentParser()
CLI.add_argument(
        "--infile", # name on the CLI
        nargs = 1,
        type = str,
        default = '../profiling_data/csv/profiling_data.csv',
)

CLI.add_argument(
        "--outdir", # name on the CLI
        nargs = 1,
        type = str,
        default = '../profiling_data/csv',
)
CLI.add_argument(
        "--width", # name on the CLI
        nargs = 1,
        type = int,
        default = 200,
)
CLI.add_argument(
        "--samples", # name on the CLI
        nargs = 1,
        type = int,
        default = 10,
)
CLI.add_argument(
        "--grid", # name on the CLI
        nargs = "*",
        type = int,
        default = [0, 5, 10],
)

args = CLI.parse_args()

df = pd.read_csv(args.infile[0])

for i in range(len(args.grid)):
    output_file = args.outdir[0] + '/filtered' + '_' + str(args.width[0]) + '_' + str(args.samples[0]) + '_'  + str(args.grid[i]) + '.csv'
    print(output_file)

    filter_condition = (
        (df['Width']        == args.width[0]) &
        (df['Samples']      == args.samples[0]) &
        (df['NumberGrid']   == args.grid[i])
    )

    filtered_df = df[filter_condition]
    filtered_df.to_csv(output_file, index=False)

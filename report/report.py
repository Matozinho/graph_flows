import streamlit as st
import pandas as pd
import json
import altair as alt

st.title(
    "Benchmark of the C++ implementation of Ford Fulkerson and Push Relabel algorithms"
)

@st.cache
def load_data():
    with open("benchmark.json", "r") as f:
        data = json.load(f)
    return data


benchmark_data = load_data()

st.caption(f"Builded as {benchmark_data['context']['library_build_type']}")

st.header("Full benchmark data")

with st.expander("See details"):
    st.json(benchmark_data)

st.header("Machine information")

st.markdown(f"`Hostname`: {benchmark_data['context']['host_name']}")
st.markdown(f"`N° cpu(s)`: {benchmark_data['context']['num_cpus']}")
st.markdown(f"`MHz/cpu`: {benchmark_data['context']['mhz_per_cpu']}")
st.markdown("`Processor`: Intel® Core™ i5-10300H")
st.markdown("`Memory`: 24 GiB")
st.markdown("`OS`: Ubuntu 22.04.2 LTS")
st.markdown(
    f"`CPU Scaling enabled`: {benchmark_data['context']['cpu_scaling_enabled']}"
)

st.subheader("Caches info")

caches = benchmark_data["context"]["caches"]
caches_column = st.columns(len(caches))

# Get cache and index
for index, cache in enumerate(caches_column):
    cache.metric(
        f"Level {caches[index]['level']}",
        f"{caches[index]['type']}",
        f"{caches[index]['size']} bits",
    )

# insertion_comparisons = benchmark_data["insertion_comparisons"]
# search_comparisons = benchmark_data["search_comparisons"]


def separate_benchmark_data(benchmarks):
    """
    Separate the benchmark data into a dictonary.
    """

    separated_benchmarks = {}

    for benchmark in benchmarks:
        if benchmark['run_type'] == 'aggregate':
            continue

        name, size = benchmark['run_name'].split('/')

        if name not in separated_benchmarks:
            separated_benchmarks[name] = {}

        if size not in separated_benchmarks[name]:
            separated_benchmarks[name][size] = {}

        separated_benchmarks[name][size] = {
            "time": benchmark["real_time"],
            "max_flow": benchmark_data[name][size]["max_flow"]
        }
        # validate if the name has the word "fulkerson" in it
        if "fulkerson" in name.lower():
            separated_benchmarks[name][size]["paths"] = benchmark_data[name][size]["paths"]

    return separated_benchmarks


separated_benchmarks = separate_benchmark_data(benchmark_data["benchmarks"])


def get_all_values_of_benchmark(name: str, key: str) -> list:
    """
    The values returned are in the format [size, value].
    """
    values = []
    for size, value in separated_benchmarks[name].items():
        values.append([int(size), value[key]])

    return values


def generate_table(name: str, operation: str, operation2: str = None):
    values = {
        "sizes": [],
        operation: [],
    }

    if operation2:
        values[operation2] = []

    for size, value in separated_benchmarks[name].items():
        values["sizes"].append(int(size))
        values[operation].append(value[operation])
        if operation2:
            values[operation2].append(value[operation2])

    operation_column = operation.replace("_", " ").capitalize()

    if operation2:
        operation2_column = operation2.replace("_", " ").capitalize()
        df = pd.DataFrame({
            "Graph Size": values["sizes"],
            operation_column: values[operation],
            operation2_column: values[operation2]
        }, columns=["Graph Size", operation_column, operation2_column])
    else:
        df = pd.DataFrame({
            "Graph Size": values["sizes"],
            operation_column: values[operation]
        }, columns=["Graph Size", operation_column])

    st.table(df)


for benchmark_name, benchmark_data in separated_benchmarks.items():
    st.header(f"{benchmark_name}")

    times_tab, max_flow_tab = st.tabs(
        ["Time", "Max flow"]
    )

    def tab(tab, key, key2=None):
        if key == "max_flow":
            generate_table(benchmark_name, key, key2)
            return

        values = get_all_values_of_benchmark(benchmark_name, key)

        df = pd.DataFrame(values, columns=["Size", "Time (ns)"])

        # Altair chart
        chart = (
            alt.Chart(df)
            .mark_line()
            .encode(
                x=alt.X("Size", title="Size of the array"),
                y=alt.Y("Time (ns)", title="Time (ns)"),
                # color="Type",
            )
        ).interactive()
        tab.altair_chart(chart, use_container_width=True)

    with times_tab:
        tab(times_tab, "time")
    with max_flow_tab:
        if "paths" in benchmark_data["10"]:
            tab(max_flow_tab, "max_flow", "paths")
        else:
            tab(max_flow_tab, "max_flow")


st.header(f"Comparison")

time_tab, max_flow_tab = st.tabs(
    ["Time", "Max flow"]
)


def generate_comparison_table(operation: str):
    values = {
        "sizes": []
    }

    for name in separated_benchmarks:
        values[name] = []

        for size, value in separated_benchmarks[name].items():
            if int(size) not in values["sizes"]:
                values["sizes"].append(int(size))
            values[name].append(value[operation])

    df = pd.DataFrame({
        "Graph Size": values["sizes"],
        "Ford Fulkerson": values["BM_Ford_Fulkerson"],
        "Push Relable": values["BM_Push_Relable"]
    }, columns=["Graph Size", "Ford Fulkerson", "Push Relable"])

    st.table(df)


def get_comparison_values_of_benchmark(operation: str) -> list:
    ford_fulkerson_data = get_all_values_of_benchmark("BM_Ford_Fulkerson", operation)
    push_relable_data = get_all_values_of_benchmark("BM_Push_Relable", operation)

    return [ford_fulkerson_data, push_relable_data]


def tab_content(tab, operation: str):
    if operation == "max_flow":
        generate_comparison_table(operation)
        return

    ford_fulkerson_values, push_relable_values = get_comparison_values_of_benchmark(operation)

    # create data frames for iterative and recursive values
    df_ford_fulkerson = pd.DataFrame(ford_fulkerson_values, columns=["size of the array", "Time"])
    df_push_relable = pd.DataFrame(push_relable_values, columns=["size of the array", "Time"])

    # add a "Type" column to each data frame
    df_ford_fulkerson["Type"] = "ford fulkerson"
    df_push_relable["Type"] = "push relable"

    # concatenate the two data frames
    df = pd.concat([df_ford_fulkerson, df_push_relable])

    # create altair chart
    chart = alt.Chart(df).mark_line().encode(
        x=alt.X("size of the array", title="Size of the array"),
        y=alt.Y("Time", title="Time (ns)"),
        color="Type"
    ).interactive()

    tab.altair_chart(chart, use_container_width=True)


with time_tab:
    tab_content(time_tab, "time")
with max_flow_tab:
    tab_content(max_flow_tab, "max_flow")


# def define_comparisons_section():
#     st.header(f"Number of comparisons")
#
#     def tab_content(tab, data):
#         formated_data = {
#             'size': [],
#             'iterative': [],
#             'recursive': []
#         }
#
#         for size, counts in data['iterative'].items():
#             if int(size) >= 50000:
#                 formated_data['size'].append(int(size))
#                 formated_data['iterative'].append(counts)
#                 formated_data['recursive'].append(data['recursive'][size])
#
#         df_data = pd.DataFrame(formated_data)
#         df = pd.melt(df_data, id_vars=['size'], value_vars=['iterative', 'recursive'])
#
#         # create the chart
#         chart = alt.Chart(df).mark_bar().encode(
#             column=alt.Column('size:O', header=alt.Header(labelOrient="bottom")),
#             x=alt.X('variable', sort=["iterative", "recursive"], axis=None),
#             y=alt.Y('value:Q'),
#             color=alt.Color('variable')
#         )
#
#         tab.altair_chart(chart)
#
#
#     insertion_tab, search_tab = st.tabs(
#         ["Insertion", "Search"]
#     )
#
#
#     with insertion_tab:
#         tab_content(insertion_tab, insertion_comparisons)
#     with search_tab:
#         tab_content(search_tab, search_comparisons)


# define_comparisons_section()

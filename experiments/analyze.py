import pandas as pd
import matplotlib.pyplot as plt

# читаем данные
df = pd.read_csv("results.csv")
df["decode_time"] = pd.to_numeric(df["decode_time"], errors="coerce")

# считаем среднее и стандартное отклонение
stats = df.groupby(["file", "type", "original_size", "compressed_size"]).agg(
    encode_mean=("encode_time", "mean"),
    encode_std=("encode_time", "std"),
    decode_mean=("decode_time", "mean"),
    decode_std=("decode_time", "std")
).reset_index()

stats["compression_ratio"] = stats["compressed_size"] / stats["original_size"] * 100

print(stats)

# степень сжатия
plt.figure()
plt.bar(stats["file"], stats["compression_ratio"])
plt.axhline(y=100, color="r", linestyle="--")
plt.title("Степень сжатия / compression_ratio")
plt.ylabel("%")
plt.xticks(rotation=45)
plt.tight_layout()
plt.savefig("graphs/compression_ratio.png")

# время сжатия
plt.figure()
plt.bar(stats["file"], stats["encode_mean"], yerr=stats["encode_std"], capsize=5)
plt.title("Время сжатия / compression time")
plt.ylabel("сек / sec")
plt.xticks(rotation=45)
plt.tight_layout()
plt.savefig("graphs/encode_time.png")

# время разжатия
plt.figure()
plt.bar(stats["file"], stats["decode_mean"], yerr =stats["decode_std"], capsize=5)
plt.title("Время разжатия / release time")
plt.ylabel("сек / sec")
plt.xticks(rotation=45)
plt.tight_layout()
plt.savefig("graphs/decode_time.png")

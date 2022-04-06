Cкачал файлы с данными экспериментов гистонов - `raw_data`.

Cкачал файлы с данными эксперимента z-dna - `curax***.bed`.

Данные по гистонам отсорстировал - `scripts/sort_raw_data.sh` -> `sorted_data`.

Анализ данных z-dna (`explore_exp_data.ipynb`) - посмотреть на длины регионов, количество регионов и тд.
Нашел юнифай-длину и количество бп на бин, записал эти данные в `global_data_params.json`.
Сгенерировал центрированные унифицированные файлы - позитивный и негативный класс (`make_target_data.ipynb`)
Сами регионы записал в `experiment_data` - `curax***.regions.bed` и `curax***.regions.discrete.bed`, отсортированные в `curax***.regions.sorted.bed` и `curax***.regions.discrete.sorted.bed` соответственнно.
С помощью `extract_features.cpp` нашел силу сигнала на полученных регионах, результаты в `feature_data`. Положительный и отрицательный классы в `positive` и `negative` соответственно. Это файлы с признаками для обучения.

template<typename Container>
void SimulateSW(const Container &C,const Container &V, Container &I);

template<typename Container>
void fillWithRandomData(Container &C, Container &V);

template<typename Container>
void SimulateSW(const Container &C, const Container &V, Container &I){
	const auto hundred = -1.0f / 100.0f;
	const auto size = V.size();
	auto k = 0;
	for (auto i = uint(0); i < size; i++) {
		auto f_acc = 0.0f;
		auto v_acc = 0.0f;
		for (auto j = uint(0); j < size; j++) {
			auto v = V[i] - V[j];
			auto f = v * expf(v * v * hundred);
			f_acc += f * C[k];
			v_acc += v * C[k];
			k++;
		}
		auto I_c = (0.8f * f_acc + 0.2f * v_acc);
		I[i]=I_c;
	}
}

template<typename Container>
void fillWithRandomData(Container &C, Container &V) {
	auto generator = std::default_random_engine{};
	auto distV = std::uniform_real_distribution<float>{2.0f, -70.0f};
	auto distC = std::uniform_real_distribution<float>{0.02f, 0.6f};
	for (auto &v : V) v = distV(generator);
	for (auto &c : C) c = distC(generator);
}

template<typename Container>
void compareResults(const Container &I_sw, const Container &I_hw){
	int successful = 0;
	float precision = 0.01;
	for (auto i = uint(0); i < I_sw.size(); i++) {
		if (fabs( I_hw[i]- I_sw[i])/I_sw[i] >= precision) {
			std::cout<< "Error in current: "<< i<<"\n";
			std::cout<< "SW current: "<< I_sw[i]<<"\n";
			std::cout<< "HW current: "<< I_hw[i]<<"\n";
			std::cout<< "%: "<< 100*(I_hw[i]- I_sw[i])/I_sw[i]<<"\n";
			successful= 1;
		}
	}

	printf("____________________________________________\n");
	if (successful == 0) {
		printf("Successful result\n");
	} else {
		printf("Error\n");
	}
	printf("____________________________________________\n");
}


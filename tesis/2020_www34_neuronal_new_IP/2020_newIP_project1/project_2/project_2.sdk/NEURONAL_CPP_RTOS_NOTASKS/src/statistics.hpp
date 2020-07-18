template<typename Container>
float average(const Container &data){
  auto sum = 0.0f;
  const auto n = static_cast<float>(data.size());
  for (const auto &samp : data) {
    sum += static_cast<float>(samp);
  }
  return sum/n;
}

template<typename Container>
float variance(const Container &data){
  const auto n = static_cast<float>(data.size());
  const auto avg = average<Container>(data);
  auto var = 0.0f;
  for (const auto &samp : data) {
    auto diff = (static_cast<float>(samp)-avg);
    var += diff*diff;
  }
  return var/(n-1);
}

template<typename Container>
float standard_deviation(const Container &data){
  auto varianceResult = variance<Container>(data);
  return sqrt(varianceResult);
}


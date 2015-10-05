#include <functional>
#include <memory>

namespace Tools
{

template<class InputType = double, class OutputType = double>
class TableCalculator
{
public:
    TableCalculator(const std::function<OutputType(InputType)>& funct,
        InputType start, InputType end, InputType step);

    OutputType Get(InputType input);
private:
    InputType start;
    InputType end;
    InputType step;
    unordered_map<size_t, OutputType> table;
};


template<class InputType, class OutputType>
TableCalculator<InputType, OutputType>::TableCalculator(
    const std::function<OutputType(InputType)>& funct,
    InputType start, InputType end, InputType step)
    : start(start), end(end), step(step)
{
    size_t enumerator = 0;
    for (InputType iter = start; iter < end; iter += step)
    {
        table[enumerator++] = funct(iter);
    }
}

template<class InputType, class OutputType>
OutputType TableCalculator<InputType, OutputType>::Get(InputType input)
{
    if (input < start) {
        return table[0]; }
    else if (input >= end) {
        return table[table.size() - 1];
    } else {
        size_t index = static_cast<size_t>((input - start) / step);
        return table[index];
    }
}

}

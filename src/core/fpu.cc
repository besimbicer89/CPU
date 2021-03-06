#include "fpu.hh"

FloatingPointUnit::FloatingPointUnit() {
    stages[0] = std::make_shared<FullRegister>(register_code::st0);
    stages[1] = std::make_shared<FullRegister>(register_code::st1);
    stages[2] = std::make_shared<FullRegister>(register_code::st2);
    stages[3] = std::make_shared<FullRegister>(register_code::st3);
    stages[4] = std::make_shared<FullRegister>(register_code::st4);
    stages[5] = std::make_shared<FullRegister>(register_code::st5);
    stages[6] = std::make_shared<FullRegister>(register_code::st6);
    stages[7] = std::make_shared<FullRegister>(register_code::st7);
}

double FloatingPointUnit::convert_double(uint64_t v) {
    return *reinterpret_cast<double *>(&v);
}

double FloatingPointUnit::convert_float(uint64_t v) {
     return *reinterpret_cast<float *>(&v);
}

uint64_t FloatingPointUnit::convert_uint64(double v) {
    return *reinterpret_cast<uint64_t *>(&v);
}

uint64_t FloatingPointUnit::convert_uint64(float v) {
    uint64_t result = 0;
    auto pointer = reinterpret_cast<float *>(&result);
    *pointer = v;
    return result;
}

register_ptr FloatingPointUnit::get_register(register_code code) {
    switch (code) {
        case register_code::st0: return stages[0];
        case register_code::st1: return stages[1];
        case register_code::st2: return stages[2];
        case register_code::st3: return stages[3];
        case register_code::st4: return stages[4];
        case register_code::st5: return stages[5];
        case register_code::st6: return stages[6];
        case register_code::st7: return stages[7];
        default: throw "provided register code does not belong to the floating point unit";
    }
}

void FloatingPointUnit::push(uint64_t v) {
    // shift values
    for (int i = 7; i > 0; i--) {
        stages[i]->set_value(stages[i - 1]->get_value());
    }

    stages[0]->set_value(v);
}

void FloatingPointUnit::pop() {
    for (int i = 0; i < 7; i++) {
        stages[i]->set_value(stages[i + 1]->get_value());
    }
}

void FloatingPointUnit::execute() {
    switch(operation) {
        case fpu_operation::fld: {
            perform_pop = false;
            auto value = src_dest->get_value();

            if (!is_double) {
                double d = convert_float(value);
                value = convert_uint64(d);
            }

            push(value);
            break;
        }

        case fpu_operation::fst: {
            auto value = stages[0]->get_value();

            if (!is_double) {
                double d = convert_double(value);
                float f = static_cast<float>(d);
                value = convert_uint64(f);
            }

            src_dest->set_value(value);
            break;
        }

        case fpu_operation::fadd: {
            double converted;

            auto value = src_dest->get_value();
            if (is_double) converted = convert_double(value);
            else converted = convert_float(value);

            auto result = converted + convert_double(stages[0]->get_value());
            auto converted_result = convert_uint64(result);

            if (is_destination) src_dest->set_value(converted_result);
            else stages[0]->set_value(converted_result);

            break;
        }

        default:
            throw "unkown or unimplemented floating point operation";
            break;
    }

    if (perform_pop) pop();
}

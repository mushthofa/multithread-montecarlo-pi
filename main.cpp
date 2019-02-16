#include <iostream>
#include <boost/program_options.hpp>
#include <random>
#include <chrono>
#include <boost/thread.hpp>
#include <vector>
#include <memory>


namespace po = boost::program_options;

auto estimatePi(size_t numPoints, double &estimate) -> void
{
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_real_distribution<double> distribution(0.0,1.0);

    size_t numIn = 0;

    for(size_t idx = 0; idx < numPoints; ++idx)
    {
        double x = distribution(generator);
        double y = distribution(generator);

        if(x*x + y*y <= 1.0)
            numIn++;
    }

    estimate = 4.0 * (static_cast<double>(numIn)/static_cast<double>(numPoints));
}

int main(int argc, char **argv) 
{
    
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help", "produce help message")
    ("es", po::value<size_t>(), "estimate size  [default 1000'000]")
    ("mt", po::value<char>(), "whether to do multithread [default y]")
    ("nt", po::value<size_t>(), "set number of threads [default 1]");
    
    
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);    

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }
    
    size_t nthreads{1};
    size_t nEstimate{1000'000};
    
    bool doMT = true;
    
    if (vm.count("nt")) 
      nthreads = vm["nt"].as<size_t>() ;
    
    if (vm.count("mt") && vm["mt"].as<char>()  == 'n') 
      doMT = false;
    
    std::vector<double> estimates;
    estimates.resize(nthreads, 0);
    
    std::cout << "Running with "<< nthreads << (doMT?(" threads "):(" iterations ")) <<std::endl;
    
    double estimate = 0;
    
    auto startTime = std::chrono::steady_clock::now();
    
    if(doMT)
    {
        
        std::vector<std::unique_ptr<boost::thread>> mythreads;
        
        for(size_t idx = 0; idx < nthreads; ++idx)
            mythreads.push_back(std::make_unique<boost::thread>(estimatePi, nEstimate, boost::ref(estimates[idx])));
        
        for(size_t idx = 0; idx < nthreads; ++idx)
            mythreads[idx]->join();
    }
    else
    {
        for(size_t idx = 0; idx < nthreads; ++idx)
            estimatePi(nEstimate, estimates[idx]);
    }
    
    
    for(size_t idx = 0; idx < nthreads; ++idx)
        estimate += estimates[idx];
    
    auto endTime = std::chrono::steady_clock::now();
    auto timeLapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()/1000.0;
    
    std::cout << "Pi = "<< estimate/nthreads << std::endl;
    std::cout << "Time = "<< timeLapsed << " s. " << std::endl;

    return 0;
}

#ifdef JOB_H
#define JOB_H

#define A_SIZE 1024 * 1024 * 4

class Job {
	public:
		void compute();

	private:
		double A[A_SIZE];

};

#endif
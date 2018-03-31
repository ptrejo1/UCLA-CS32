const int SIZE = 100;

class Stack{

public:
	Stack(){
		m_top = 0;
	}

	void push(int val){
		if (m_top >= SIZE) return;
		m_stack[m_top] = val;
		m_top += 1;
	}

	int pop(){
		if(m_top == 0) return -1;
		m_top -= 1;
		return m_stack[m_top];
	}

	bool is_empty(void);
	int peek_top();

private:
	int m_stack[SIZE];
	int m_top;
}

int main(void)
{
	

	return 0;
}
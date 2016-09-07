__author__ = 'Miroslaw Horbal'
__email__ = 'miroslaw@gmail.com'
__date__ = 'December 1, 2013'
import sys

# Notes
# Dec 2, 2013: Updated the parser to fix the bug noticed by kinnskogr
#              on the kaggle forums. The fix involves directly assigning
#              clicks to query objects
# Dec 22, 2013: Updated the parser to include an extra yield statement at
#		the end of the end of the main parser for-loop 
# 		Thanks to kaggle user's Shan and Sebastian Butterweck for finding the bug!

def parse_sessions(file):
    print("AAAA")
    """
    A generator that yields session objects from file
    
    Each session object has the form:
        { SessionID: int,
          Day: int,
          USERID: int,
          Query: [{ TimePassed: int,
                   SERPID: int,
                   QueryID: int,
                   ListOfTerms: [TermID_1, ...],
                   Clicks: [{ TimePassed: int, SERPID: int, URLID: int }, ...],
                   URL_DOMAIN: [(URLID_1, DomainID_1), ...] } }, ...]
    """
    print("AAAA")
    s = None
    for line in file:
        sline = line.strip().split('\t')
        if len(sline) == 4 and s is not None:
            yield s
        
        if len(sline) == 4:
            sid, tor, day, uid = sline
            s = create_session(sid, day, uid)
        elif len(sline) == 16:
            sid, tp, tor, serpid, quid, lot = sline[:6]
            lou = sline[6:]
            s['Query'].append(create_query(tp, serpid, quid, lot, lou))
        elif len(sline) == 5:
            sid, tp, tor, serpid, urlid = sline
            cl = create_click(tp, serpid, urlid)
            s['Query'][-1]['Clicks'][cl['URLID']] = cl
    yield s

def create_session(session_id, day, user_id):
    return { 'SessionID': int(session_id), 
              'Day': int(day),
              'USERID': int(user_id),
              'Query': [] }

def create_query(time_passed, serp_id, query_id, list_of_terms, url_list):
    return { 'TimePassed': int(time_passed),
              'SERPID': int(serp_id),
              'QueryID': int(query_id),
              'Clicks': {},
              'ListOfTerms': [int(t) for t in list_of_terms.split(',')],
              'URL_DOMAIN': [tuple(int(ui) for ui in u.split(','))
                                for u in url_list] }

def create_click(time_passed, serp_id, url_id):
    return { 'TimePassed': int(time_passed),
              'SERPID': int(serp_id),
              'URLID': int(url_id) }


def Click_types(URLSDOMAINS, clicks):
    serp_size = 10
    result = []
    n_clicks_seen = 0
    n_clicks = len(clicks)
    #url, type, rank, domain
    times = []
    for t in clicks:
        times.append([clicks[t]['TimePassed'], clicks[t]['URLID']])
    times.sort(key = lambda x : x[0])
    least_rank = 0
    types = {}
    for r,t in enumerate(times):
        type = 0
        if (r == len(times) - 1):
            type = 2
        else :
            if (times[r+1][0] - t[0] >= 400):
                type = 2
            elif (times[r+1][0] - t[0] >= 50):
                type = 1
        types[t[1]] = type
    for r,i in enumerate(URLSDOMAINS):
        type = 0
        if i[0] in types:
            type = types[i[0]] 
        result.append([i[0], type, r, i[1]])
    return result


def printSession(s, q_n):
    result = ''
    st = str(s['USERID'])
    st += "\t" + str(s["SessionID"])
    st += "\t" + str(s['Query'][q_n]["QueryID"])
    st += "\t" + str(s['Day'])
    urls = Click_types(s['Query'][q_n]['URL_DOMAIN'], s['Query'][q_n]['Clicks'])
    serp_size = 10
    for i in range(serp_size):
        st1 = st + "\t"
        st1 += "\t".join(str(u) for u in urls[i])
        terms = s['Query'][q_n]['ListOfTerms']
        if len(terms) > 5:
            terms = terms[:5]
        n_terms = len(terms)
        for i in range(5 - n_terms):
            terms.append('0')
        st1 += '\t'
        st1 += "\t".join(str(t) for t in terms)
        result += st1 + "\n"
    return result



def main():
    with open(sys.argv[1]) as file, open(sys.argv[2], 'w') as res_file:
        ss = parse_sessions(file)
        j = 0
        for s in ss:
            if (j % 10000 == 0):
                print j
            j += 1
            session_len = len(s['Query'])
            for i in range(session_len):
                res_file.write(printSession(s, i))
    print("AAAAA")

main()
